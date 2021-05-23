#define EIGEN_STACK_ALLOCATION_LIMIT 0

#include "Eigen/Geometry"
#include "mathFunctions.h"
#include "Albedo.h"

using namespace Eigen;

Vector<float, 2> radiansToIndices(Vector3f vectorSpherical) {
    const float PI = 3.1415926535897;

    float azimuthalAngle = vectorSpherical(1);
    float polarAngle = vectorSpherical(2);

    float dx = 2 * PI / reflectivityDataColumns;
    float dy = PI / reflectivityDataRows;

    uint8_t i = round((PI - dy / 2 - polarAngle) / dy) + 1;
    uint8_t j = round((azimuthalAngle + PI - dx / 2) / dx) + 1;

    if (i == 0) {
        i = 1;
    }
    if (j == 0) {
        j = 1;
    }

    Vector<float, 2> indices;
    indices(0) = i;
    indices(1) = j;

    return indices;
}

Vector<float, 2> indicesToRadians(uint8_t i, uint8_t j) {

    const float PI = 3.1415926535897;

    float dx = 2 * PI / reflectivityDataColumns;
    float dy = PI / reflectivityDataRows;

    float phi = PI - dy / 2 - (i - 1) * dy;
    float theta = (j - 1) * dx - PI + dx / 2;

    Vector<float, 2> radians;
    radians(0) = theta;
    radians(1) = phi;

    return radians;
}

float calculateCellArea(uint8_t i, uint8_t j) {

    const float EMR = 6371.01e3;
    const float PI = 3.1415926535897;
    const float degreesToRad = PI / 180;

    Vector<float, 2> radians = indicesToRadians(i, j);

    float dphi = (180 / reflectivityDataRows) * degreesToRad;
    float dtheta = (360 / reflectivityDataColumns) * degreesToRad;

    float phimax = radians(1) + dphi / 2;
    float phimin = radians(1) - dphi / 2;

    float A = EMR * EMR * dtheta * (cos(phimin) - cos(phimax));
    return A;
}

float gridAngle(uint8_t loopI, uint8_t loopJ, uint8_t sunIndexI, uint8_t sunIndexJ) {

    Vector<float, 2> loopRadians = indicesToRadians(loopI, loopJ);
    Vector<float, 2> sunRadians = indicesToRadians(sunIndexI, sunIndexJ);

    float rho = acos(sin(loopRadians(1)) * sin(sunRadians(1)) * cos(loopRadians(0) - sunRadians(0)) +
                     cos(loopRadians(1)) * cos(sunRadians(1)));

    return rho;
}

Matrix<float, reflectivityDataRows, reflectivityDataColumns> albedo(Vector3f satellite, Vector3f sunPosition,
                                                                    Matrix<float, reflectivityDataRows, reflectivityDataColumns> reflectivityData) {

    const float PI = 3.1415926535897;
    const float solarIrradiance = 1;
    const float EMR = 6371.01e3;

    Vector3f satelliteSpherical = cartesianToSpherical(satellite);
    Vector3f sunPositionSpherical = cartesianToSpherical(sunPosition);

    satelliteSpherical(1) = PI / 2 - satelliteSpherical(1);
    sunPositionSpherical(1) = PI / 2 - sunPositionSpherical(1);

    Vector<float, 2> sunIndices = radiansToIndices(sunPositionSpherical);

    Vector3f grid;
    Matrix<float, reflectivityDataRows, reflectivityDataColumns> albedo;

    for (int i = 0; i < reflectivityDataRows; i++) {
        for (int j = 0; j < reflectivityDataColumns; j++) {

            float angleOfIncidentSolarIrradiance = gridAngle(i, j, sunIndices(0), sunIndices(1));

            if (angleOfIncidentSolarIrradiance > PI / 2) {
                angleOfIncidentSolarIrradiance = PI / 2;
            }

            float incidentPower = solarIrradiance * calculateCellArea(i, j) * cos(angleOfIncidentSolarIrradiance);
            Vector<float, 2> gridRadians;
            gridRadians = indicesToRadians(i, j);
            float gridTheta = gridRadians(0);
            float gridPhi = gridRadians(1);

            Vector3f cartesianVector;
            cartesianVector(0) = gridTheta;
            cartesianVector(1) = PI / 2 - gridPhi;
            cartesianVector(2) = EMR;

            grid = SphericalToCartesian(cartesianVector);

            float satelliteDistance = (satellite - grid).norm();

            // acos(((sat - grid) / satdist)' * grid / norm(grid))
            float phiOut = acos( (((satellite - grid) / satelliteDistance).transpose()).dot(grid) / grid.norm());
            float pOut = incidentPower * reflectivityData(i, j) * cos(phiOut) /
                         (PI * satelliteDistance * satelliteDistance);
            albedo(i, j) = pOut;
        }
    }

    return albedo;
}