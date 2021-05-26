#define EIGEN_STACK_ALLOCATION_LIMIT 0

#include "Eigen/Geometry"
#include "Eigen/Dense"
#include "mathFunctions.h"
#include "Albedo.h"

using namespace Eigen;

Vector<int16_t, 2> radiansToIndices(float theta, float phi) {

    float dx = 2 * M_PI / reflectivityDataColumns;
    float dy = M_PI / reflectivityDataRows;

    int16_t i = round((M_PI - dy / 2 - phi) / dy);
    int16_t j = round((theta + M_PI - dx / 2) / dx);

    if (i == -1) {
        i = 0;
    }
    if (j == -1) {
        j = 0;
    }

    Vector<int16_t, 2> indices;
    indices(0) = i;
    indices(1) = j;

    return indices;
}

Vector2f indicesToRadians(uint16_t i, uint16_t j) {

    float dx = 2 * M_PI / reflectivityDataColumns;
    float dy = M_PI / reflectivityDataRows;

    float phi = M_PI - dy / 2 - i * dy;
    float theta = j * dx - M_PI + dx / 2;

    Vector2f radians;
    radians(0) = theta;
    radians(1) = phi;

    return radians;
}

float calculateCellArea(uint16_t i, uint16_t j) {

    const float EMR = 6371.01e3;

    Vector2f radians = indicesToRadians(i, j);

    float dphi = deg2rad(180 / static_cast<float> (reflectivityDataRows));
    float dtheta = deg2rad(360 / static_cast<float> (reflectivityDataColumns));

    float phiMax = radians(1) + dphi / 2;
    float phiMin = radians(1) - dphi / 2;

    float area = EMR * EMR * dtheta * (cos(phiMin) - cos(phiMax));
    return area;
}

float gridAngle(uint16_t loopI, uint16_t loopJ, int16_t sunIndexI, int16_t sunIndexJ) {

    Vector2f loopRadians = indicesToRadians(loopI, loopJ);
    Vector2f sunRadians = indicesToRadians(sunIndexI, sunIndexJ);

    float rho = acos(sin(loopRadians(1)) * sin(sunRadians(1)) * cos(loopRadians(0) - sunRadians(0)) +
            cos(loopRadians(1)) * cos(sunRadians(1)));

    return rho;
}

Matrix<float, reflectivityDataRows, reflectivityDataColumns>
calculateAlbedo(const Vector3f &satellite, const Vector3f &sunPosition,
                const Matrix<float, reflectivityDataRows, reflectivityDataColumns> &reflectivityData) {

    const float solarIrradiance = 1;
    const float EMR = 6371.01e3;

    Vector3f satelliteSpherical = cartesianToSpherical(satellite);
    Vector3f sunPositionSpherical = cartesianToSpherical(sunPosition);

    satelliteSpherical(1) = M_PI / 2 - satelliteSpherical(1);
    sunPositionSpherical(1) = M_PI / 2 - sunPositionSpherical(1);

    Vector<int16_t, 2> sunIndices = radiansToIndices(sunPositionSpherical(0), sunPositionSpherical(1));

    Vector3f grid;
    Matrix<float, reflectivityDataRows, reflectivityDataColumns> albedo;

    for (uint16_t i = 0; i < reflectivityDataRows; i++) {
        for (uint16_t j = 0; j < reflectivityDataColumns; j++) {

            float angleOfIncidentSolarIrradiance = gridAngle(i, j, sunIndices(0), sunIndices(1));

            if (angleOfIncidentSolarIrradiance > M_PI / 2) {
                angleOfIncidentSolarIrradiance = M_PI / 2;
            }

            float incidentPower = solarIrradiance * calculateCellArea(i, j) * cos(angleOfIncidentSolarIrradiance);
            Vector2f gridRadians = indicesToRadians(i, j);
            float gridTheta = gridRadians(0);
            float gridPhi = gridRadians(1);

            Vector3f vectorSpherical;
            vectorSpherical(0) = gridTheta;
            vectorSpherical(1) = M_PI / 2 - gridPhi;
            vectorSpherical(2) = EMR;

            grid = sphericalToCartesian(vectorSpherical);

            float satelliteDistance = (satellite - grid).norm();

            float satelliteGridAngle = acos(
                    (((satellite - grid) / satelliteDistance).transpose()).dot(grid) / grid.norm());
            float pOut = incidentPower * reflectivityData(i, j) * cos(satelliteGridAngle) /
                         (M_PI * satelliteDistance * satelliteDistance);
            albedo(i, j) = pOut;
        }
    }

    return albedo;
}