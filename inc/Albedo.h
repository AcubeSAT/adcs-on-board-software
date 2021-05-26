#ifndef ADCS_ONBOARD_SOFTWARE_ALBEDO_H
#define ADCS_ONBOARD_SOFTWARE_ALBEDO_H

#include "Eigen/Geometry"

using namespace Eigen;

const uint16_t reflectivityDataRows = 180;
const uint16_t reflectivityDataColumns = 288;

/**
 * Transforms radians to TOMS reflectivity matrix indices
 * @param theta reflectivity data point's latitude
 * @param phi reflectivity data point's longitude
 * @return vector containing TOMS reflectivity matrix indices
 */
Vector<int16_t, 2> radiansToIndices(float theta, float phi);

/**
 * Transforms TOMS reflectivity matrix indices to radians
 * @param i TOMS reflectivity matrix index
 * @param j TOMS reflectivity matrix index
 * @return vector containing reflectivity data point's latitude and longitude
 */
Vector2f indicesToRadians(uint16_t i, uint16_t j);

/**
 * Calculates the area of a cell with indices i, j
 * @param i index i
 * @param j index j
 * @return area of a cell with indices i, j
 */
float calculateCellArea(uint16_t i, uint16_t j);

/**
 * Calculates the angle between two grid index pairs
 * @param loopI reflectivity grid index i
 * @param loopJ reflectivity grid index j
 * @param sunIndexI sun grid index i
 * @param sunIndexJ sun grid index j
 * @return angle between two grid index pairs
 */
float gridAngle(uint16_t loopI, uint16_t loopJ, int16_t sunIndexI, int16_t sunIndexJ);

/**
 * Calculates the sunlight reflected off the Earth's surface
 * @param satellite satellite position in ECEF frame
 * @param sunPosition sun position in ECEF frame
 * @param reflectivityData Earth surface reflectivity data from TOMS project
 * @return sunlight reflected off the Earth's surface
 */
Matrix<float, reflectivityDataRows, reflectivityDataColumns>
calculateAlbedo(const Vector3f &satellite, const Vector3f &sunPosition,
                const Matrix<float, reflectivityDataRows, reflectivityDataColumns> &reflectivityData);


#endif //ADCS_ONBOARD_SOFTWARE_ALBEDO_H
