#ifndef ADCS_ONBOARD_SOFTWARE_ALBEDO_H
#define ADCS_ONBOARD_SOFTWARE_ALBEDO_H

#include "Eigen/Geometry"

using namespace Eigen;

const uint16_t reflectivityDataRows = 180;
const uint16_t reflectivityDataColumns = 288;

Vector<int16_t, 2> radiansToIndices(float theta, float phi);

Vector2f indicesToRadians(uint16_t i, uint16_t j);

float calculateCellArea(uint16_t i, uint16_t j);

float gridAngle(uint16_t loopI, uint16_t loopJ, int16_t sunIndexI, int16_t sunIndexJ);

Matrix<float, reflectivityDataRows, reflectivityDataColumns>
calculateAlbedo(const Vector3f &satellite, const Vector3f &sunPosition,
                const Matrix<float, reflectivityDataRows, reflectivityDataColumns> &reflectivityData);


#endif //ADCS_ONBOARD_SOFTWARE_ALBEDO_H
