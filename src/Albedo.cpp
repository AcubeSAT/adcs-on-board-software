#include "Eigen/Geometry"
#include "Eigen/Dense"
#include "MathFunctions.hpp"
#include "Albedo.hpp"

using namespace Eigen;

float albedo::calculateCellArea(const int16_t i, const int16_t j) {
    Vector2f radians = albedo::indicesToRadians(i, j);

    const float deltaPhi = deg2rad(180 / static_cast<float>(ReflectivityDataRows));
    const float deltaTheta = deg2rad(360 / static_cast<float>(ReflectivityDataColumns));

    const float phiMax = radians(1) + deltaPhi / 2;
    const float phiMin = radians(1) - deltaPhi / 2;

    const float area = EMR * EMR * deltaTheta * (cos(phiMin) - cos(phiMax));
    return area;
}

float albedo::gridAngle(const int16_t loopI, const int16_t loopJ, const int16_t sunIndexI, const int16_t sunIndexJ) {
    const Vector2f loopRadians = albedo::indicesToRadians(loopI, loopJ);
    const Vector2f sunRadians = albedo::indicesToRadians(sunIndexI, sunIndexJ);

    const float angle = acos(sin(loopRadians(1)) * sin(sunRadians(1)) * cos(loopRadians(0) - sunRadians(0)) +
                       cos(loopRadians(1)) * cos(sunRadians(1)));

    return angle;
}

Matrix<float, ReflectivityDataRows, ReflectivityDataColumns>
calculateAlbedo(const Vector3f &satellite, const Vector3f &sunPosition,
                const Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> &reflectivityData) {
    const float solarIrradiance = 1;

    Vector3f sunPositionSpherical = cartesianToSpherical(sunPosition);
    sunPositionSpherical(1) = PI / 2 - sunPositionSpherical(1);

    const Vector<int16_t, 2> sunIndices = albedo::radiansToIndices(sunPositionSpherical(0), sunPositionSpherical(1));

    Vector3f grid;
    Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> albedo;

    for (int16_t i = 0; i < ReflectivityDataRows; i++) {
        for (int16_t j = 0; j < ReflectivityDataColumns; j++) {

            float angleOfIncidentSolarIrradiance = albedo::gridAngle(i, j, sunIndices(0), sunIndices(1));

            angleOfIncidentSolarIrradiance = std::min(angleOfIncidentSolarIrradiance, static_cast<float>(PI / 2));

            const float incidentPower =
                    solarIrradiance * albedo::calculateCellArea(i, j) * cos(angleOfIncidentSolarIrradiance);
            const Vector2f gridRadians = albedo::indicesToRadians(i, j);
            const float gridTheta = gridRadians(0);
            const float gridPhi = gridRadians(1);

            grid = sphericalToCartesian(Vector3f(gridTheta, PI / 2 - gridPhi, EMR));

            const float satelliteDistance = (satellite - grid).norm();

            const float satelliteGridAngle = acos(
                    (((satellite - grid) / satelliteDistance).transpose()).dot(grid) / grid.norm());
            const float pOut = incidentPower * reflectivityData(i, j) * cos(satelliteGridAngle) /
                         (PI * satelliteDistance * satelliteDistance);
            albedo(i, j) = pOut;
        }
    }

    return albedo;
}