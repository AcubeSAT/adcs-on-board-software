#define EIGEN_STACK_ALLOCATION_LIMIT 0

#include "Eigen/Geometry"
#include "Eigen/Dense"
#include "mathFunctions.h"
#include "Albedo.h"

using namespace Eigen;

Vector<int8_t, 2> Albedo::radiansToIndices(float theta, float phi) {

    const float PI = 3.1415926535897;

    float dx = 2 * PI / reflectivityDataColumns;
    float dy = PI / reflectivityDataRows;

    int8_t i = round((PI - dy / 2 - phi) / dy) + 1;
    int8_t j = round((theta + PI - dx / 2) / dx) + 1;

    if (i == 0) {
        i = 1;
    }
    if (j == 0) {
        j = 1;
    }

    Vector<int8_t, 2> indices;
    indices(0) = i;
    indices(1) = j;

    return indices;
}

Vector2f Albedo::indicesToRadians(int8_t i, int8_t j) {

    const float PI = 3.1415926535897;

    float dx = 2 * PI / reflectivityDataColumns;
    float dy = PI / reflectivityDataRows;

    float phi = PI - dy / 2 - (i - 1) * dy;
    float theta = (j - 1) * dx - PI + dx / 2;

    Vector2f radians;
    radians(0) = theta;
    radians(1) = phi;

    return radians;
}

float Albedo::calculateCellArea(int8_t i, int8_t j) {

    const float EMR = 6371.01e3;
    const float PI = 3.1415926535897;
    const float degreesToRad = PI / 180;

    Vector2f radians = indicesToRadians(i, j);

    float dphi = (180 / reflectivityDataRows) * degreesToRad;
    float dtheta = (360 / reflectivityDataColumns) * degreesToRad;

    float phimax = radians(1) + dphi / 2;
    float phimin = radians(1) - dphi / 2;

    float A = EMR * EMR * dtheta * (cos(phimin) - cos(phimax));
    return A;
}

float Albedo::gridAngle(int8_t loopI, int8_t loopJ, int8_t sunIndexI, int8_t sunIndexJ) {

    Vector2f loopRadians = indicesToRadians(loopI, loopJ);
    Vector2f sunRadians = indicesToRadians(sunIndexI, sunIndexJ);

    float rho = acos(sin(loopRadians(1)) * sin(sunRadians(1)) * cos(loopRadians(0) - sunRadians(0)) +
                     cos(loopRadians(1)) * cos(sunRadians(1)));

    return rho;
}

Matrix<float, reflectivityDataRows, reflectivityDataColumns>
Albedo::calculateAlbedo(Vector3f satellite, Vector3f sunPosition,
                        Matrix<float, reflectivityDataRows, reflectivityDataColumns> reflectivityData) {

    const float PI = 3.1415926535897;
    const float solarIrradiance = 1;
    const float EMR = 6371.01e3;

    Vector3f satelliteSpherical = cartesianToSpherical(satellite);
    Vector3f sunPositionSpherical = cartesianToSpherical(sunPosition);

    satelliteSpherical(1) = PI / 2 - satelliteSpherical(1);
    sunPositionSpherical(1) = PI / 2 - sunPositionSpherical(1);

    Vector<int8_t, 2> sunIndices = radiansToIndices(sunPositionSpherical(0), sunPositionSpherical(1));

    Vector3f grid;
    Matrix<float, reflectivityDataRows, reflectivityDataColumns> albedo;

    for (int i = 0; i < reflectivityDataRows; i++) {
        for (int j = 0; j < reflectivityDataColumns; j++) {

            float angleOfIncidentSolarIrradiance = gridAngle(i, j, sunIndices(0), sunIndices(1));

            if (angleOfIncidentSolarIrradiance > PI / 2) {
                angleOfIncidentSolarIrradiance = PI / 2;
            }
            float incidentPower = solarIrradiance * calculateCellArea(i, j) * cos(angleOfIncidentSolarIrradiance);
            Vector2f gridRadians = indicesToRadians(i, j);
            float gridTheta = gridRadians(0);
            float gridPhi = gridRadians(1);

            Vector3f vectorSpherical;
            vectorSpherical(0) = gridTheta;
            vectorSpherical(1) = PI / 2 - gridPhi;
            vectorSpherical(2) = EMR;

            grid = sphericalToCartesian(vectorSpherical);

            float satelliteDistance = (satellite - grid).norm();

            float phiOut = acos((((satellite - grid) / satelliteDistance).transpose()).dot(grid) / grid.norm());
            float pOut = incidentPower * reflectivityData(i, j) * cos(phiOut) /
                         (PI * satelliteDistance * satelliteDistance);
            albedo(i, j) = pOut;
        }
    }

    return albedo;
}