#include "Eigen/Geometry"
#include "Eigen/Dense"
#include "mathFunctions.h"
#include "Albedo.h"

using namespace Eigen;

inline Vector<int16_t, 2> albedo::radiansToIndices(float theta, float phi) {

    float dx = 2 * M_PI / ReflectivityDataColumns;
    float dy = M_PI / ReflectivityDataRows;

    int16_t i = static_cast<int16_t>(round((M_PI - dy / 2 - phi) / dy));
    int16_t j = static_cast<int16_t>(round((theta + M_PI - dx / 2) / dx));

    if (i == -1) {
        i = 0;
    }
    if (j == -1) {
        j = 0;
    }

    return {i, j};
}

inline Vector2f albedo::indicesToRadians(int16_t i, int16_t j) {

    float dx = 2 * M_PI / ReflectivityDataColumns;
    float dy = M_PI / ReflectivityDataRows;

    float phi = M_PI - dy / 2 - i * dy;
    float theta = j * dx - M_PI + dx / 2;

    return {theta, phi};
}

float albedo::calculateCellArea(int16_t i, int16_t j) {

    Vector2f radians = albedo::indicesToRadians(i, j);

    float deltaPhi = deg2rad(180 / static_cast<float>(ReflectivityDataRows));
    float deltaTheta = deg2rad(360 / static_cast<float>(ReflectivityDataColumns));

    float phiMax = radians(1) + deltaPhi / 2;
    float phiMin = radians(1) - deltaPhi / 2;

    float area = EMR * EMR * deltaTheta * (cos(phiMin) - cos(phiMax));
    return area;
}

float albedo::gridAngle(int16_t loopI, int16_t loopJ, int16_t sunIndexI, int16_t sunIndexJ) {

    Vector2f loopRadians = albedo::indicesToRadians(loopI, loopJ);
    Vector2f sunRadians = albedo::indicesToRadians(sunIndexI, sunIndexJ);

    float angle = acos(sin(loopRadians(1)) * sin(sunRadians(1)) * cos(loopRadians(0) - sunRadians(0)) +
                       cos(loopRadians(1)) * cos(sunRadians(1)));

    return angle;
}

Matrix<float, ReflectivityDataRows, ReflectivityDataColumns>
calculateAlbedo(const Vector3f &satellite, const Vector3f &sunPosition,
                const Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> &reflectivityData) {

    const float solarIrradiance = 1;

    Vector3f sunPositionSpherical = cartesianToSpherical(sunPosition);
    sunPositionSpherical(1) = M_PI / 2 - sunPositionSpherical(1);

    Vector<int16_t, 2> sunIndices = albedo::radiansToIndices(sunPositionSpherical(0), sunPositionSpherical(1));

    Vector3f grid;
    Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> albedo;

    for (int16_t i = 0; i < ReflectivityDataRows; i++) {
        for (int16_t j = 0; j < ReflectivityDataColumns; j++) {

            float angleOfIncidentSolarIrradiance = albedo::gridAngle(i, j, sunIndices(0), sunIndices(1));

            if (angleOfIncidentSolarIrradiance > M_PI / 2) {
                angleOfIncidentSolarIrradiance = M_PI / 2;
            }

            float incidentPower = solarIrradiance * albedo::calculateCellArea(i, j) * cos(angleOfIncidentSolarIrradiance);
            Vector2f gridRadians = albedo::indicesToRadians(i, j);
            float gridTheta = gridRadians(0);
            float gridPhi = gridRadians(1);

            grid = sphericalToCartesian(Vector3f(gridTheta, M_PI / 2 - gridPhi, EMR));

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