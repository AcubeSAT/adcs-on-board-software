#include "Eigen/Geometry"
#include "Eigen/Dense"
#include "mathFunctions.h"
#include "Albedo.h"

using namespace Eigen;

Albedo::Albedo() {

    Vector<float, ReflectivityDataColumns> indices = Vector<float, ReflectivityDataColumns>::LinSpaced(
            ReflectivityDataColumns, 0, ReflectivityDataColumns - 1);
    Vector<float, ReflectivityDataColumns> ones = Vector<float, ReflectivityDataColumns>::LinSpaced(
            ReflectivityDataColumns, 1, 1);

    float dx = 2 * M_PI / ReflectivityDataColumns;
    float dy = M_PI / ReflectivityDataRows;

    this->phis = (M_PI - dy / 2) * ones(seq(0, ReflectivityDataRows-1)) - indices(seq(0, ReflectivityDataRows-1)) * dy;
    this->thetas = indices(seq(0, ReflectivityDataColumns-1)) * dx - (M_PI + dx / 2)*ones(seq(0, ReflectivityDataColumns-1));

    /**
     *  BUG: this->thetas(1) = theta for j = 0
     *  desired: this->thetas(1) = theta for j = 1
     */

    float deltaPhi = deg2rad(180 / static_cast<float>(ReflectivityDataRows));
    float deltaTheta = deg2rad(360 / static_cast<float>(ReflectivityDataColumns));

    Vector<float, ReflectivityDataRows> phiMax = this->phis + (deltaPhi / 2) * ones(seq(0, ReflectivityDataRows-1));
    Vector<float, ReflectivityDataRows> phiMin = this->phis - (deltaPhi / 2)  * ones(seq(0, ReflectivityDataRows-1));

    /** BUG: this->cellAreas size isn't correct. Data aren't stored properly **/
    this->cellAreas = EMR * EMR * deltaTheta * (phiMin.array().cos() - phiMax.array().cos());

}
/**
 * NOTES:
 * area of each cell is now calculated in the constructor and stored as a vector.
 * This method is now useless.
 */

float Albedo::calculateCellArea(int16_t i, int16_t j) {

    Vector2f radians = indicesToRadians(i, j);

    float deltaPhi = deg2rad(180 / static_cast<float>(ReflectivityDataRows));
    float deltaTheta = deg2rad(360 / static_cast<float>(ReflectivityDataColumns));

    float phiMax = radians(1) + deltaPhi / 2;
    float phiMin = radians(1) - deltaPhi / 2;

    float area = EMR * EMR * deltaTheta * (cos(phiMin) - cos(phiMax));
    return area;
}

/**
 * NOTES:
 * indices depend on reflectivity data size, which is taken as constant since we use TOMS project refl. data
 * all indices are now calculated in the constructor and stored as two vectors.
 * This method is now useless.
 */
Vector2f Albedo::indicesToRadians(int16_t i, int16_t j) {

    float dx = 2 * M_PI / ReflectivityDataColumns;
    float dy = M_PI / ReflectivityDataRows;

    float phi = M_PI - dy / 2 - i * dy;
    float theta = j * dx - M_PI + dx / 2;

    return {theta, phi};
}

Vector<int16_t, 2> Albedo::radiansToIndices(float theta, float phi) {

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

float Albedo::gridAngle(int16_t loopI, int16_t loopJ, int16_t sunIndexI, int16_t sunIndexJ) {

    Vector2f loopRadians = indicesToRadians(loopI, loopJ);
    Vector2f sunRadians = indicesToRadians(sunIndexI, sunIndexJ);

    float angle = acos(sin(loopRadians(1)) * sin(sunRadians(1)) * cos(loopRadians(0) - sunRadians(0)) +
                       cos(loopRadians(1)) * cos(sunRadians(1)));

    return angle;
}

/**
 * NOTES:
 * Returns a matrix with elements equal to 1 when the cell is in our fov and 0 when it isn't.
 */
Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> Albedo::earthFieldOfView(Vector3f satelliteSpherical) {

    float theta0 = satelliteSpherical(0);
    float phi0 = satelliteSpherical(1);

    if (satelliteSpherical(2) < EMR) {
        satelliteSpherical(2) = satelliteSpherical(3) + EMR;
    }

    float rho = acos(EMR / satelliteSpherical(2));

    Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> earthFieldOfView = Matrix<float, ReflectivityDataRows, ReflectivityDataColumns>::Zero();

    for (int16_t i = 0; i < ReflectivityDataRows; i++) {
        for (int16_t j = 0; j < ReflectivityDataColumns; j++) {

            float radialDistance = acos(sin(phi0) * sin(phis(i)) * cos(theta0 - thetas(j)) + cos(phi0) * cos(phis(i)));

            if (radialDistance <= rho) {
                earthFieldOfView(i, j) = 0;
            }
        }
    }

    return earthFieldOfView;
}

/**
 * NOTES:
 * Since albedo is considered as a nadir vector with norm equal to the sum of each Earth cell's albedo,
 * loading all these elements to a matrix is less efficient than calculating directly the sum.
 */
float
Albedo::calculateAlbedo(const Vector3f &satellite, const Vector3f &sunPosition,
                        const Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> &reflectivityData) {

    const float solarIrradiance = 1;

    Vector3f sunPositionSpherical = cartesianToSpherical(sunPosition);
    sunPositionSpherical(1) = M_PI / 2 - sunPositionSpherical(1);

    Vector<int16_t, 2> sunIndices = radiansToIndices(sunPositionSpherical(0), sunPositionSpherical(1));

    float albedo = 0;
    Vector3f grid;

    Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> earthFov = earthFieldOfView(
            cartesianToSpherical(satellite));

    for (int16_t i = 0; i < ReflectivityDataRows; i++) {
        for (int16_t j = 0; j < ReflectivityDataColumns; j++) {

            if (earthFov(i, j)) {
                float angleOfIncidentSolarIrradiance = gridAngle(i, j, sunIndices(0), sunIndices(1));

                if (angleOfIncidentSolarIrradiance > M_PI / 2) {
                    angleOfIncidentSolarIrradiance = M_PI / 2;
                }

                float incidentPower =
                        solarIrradiance * calculateCellArea(i, j) * cos(angleOfIncidentSolarIrradiance);

                float gridTheta = this->thetas(j);
                float gridPhi = this->phis(i);

                grid = sphericalToCartesian(Vector3f(gridTheta, M_PI / 2 - gridPhi, EMR));

                float satelliteDistance = (satellite - grid).norm();

                float satelliteGridAngle = acos(
                        (((satellite - grid) / satelliteDistance).transpose()).dot(grid) / grid.norm());
                float pOut = incidentPower * reflectivityData(i, j) * cos(satelliteGridAngle) /
                             (M_PI * satelliteDistance * satelliteDistance);
                albedo += pOut;
            }
        }
    }

    return albedo;
}