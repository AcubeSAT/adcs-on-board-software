#ifndef ADCS_ONBOARD_SOFTWARE_ALBEDO_H
#define ADCS_ONBOARD_SOFTWARE_ALBEDO_H

#include "Eigen/Geometry"
#include <algorithm>

using namespace Eigen;

const int16_t ReflectivityDataRows = 180;
const int16_t ReflectivityDataColumns = 288;

const static float EMR = 6371.01e3;

namespace albedo {
/**
 * Transforms radians to TOMS reflectivity matrix indices
 * @param theta reflectivity data point's latitude
 * @param phi reflectivity data point's longitude
 * @return vector containing TOMS reflectivity matrix indices
 */
    inline Vector<int16_t, 2> radiansToIndices(float theta, float phi) {
        float dx = 2 * M_PI / ReflectivityDataColumns;
        float dy = M_PI / ReflectivityDataRows;

        int16_t i = static_cast<int16_t>(round((M_PI - dy / 2 - phi) / dy));
        int16_t j = static_cast<int16_t>(round((theta + M_PI - dx / 2) / dx));

        i = std::max(static_cast<int16_t>(0), i);
        j = std::max(static_cast<int16_t>(0), j);

        return {i, j};
    }

/**
 * Transforms TOMS reflectivity matrix indices to radians
 * @param i TOMS reflectivity matrix index
 * @param j TOMS reflectivity matrix index
 * @return vector containing reflectivity data point's latitude and longitude
 */
    inline Vector2f indicesToRadians(int16_t i, int16_t j) {
        float dx = 2 * M_PI / ReflectivityDataColumns;
        float dy = M_PI / ReflectivityDataRows;

        float phi = M_PI - dy / 2 - i * dy;
        float theta = j * dx - M_PI + dx / 2;

        return {theta, phi};
    }

/**
 * Calculates the area of a cell with indices i, j
 * @param i index i
 * @param j index j
 * @return area of a cell with indices i, j
 */
    float calculateCellArea(int16_t i, int16_t j);

/**
 * Calculates the angle between two grid index pairs
 * @param loopI reflectivity grid index i
 * @param loopJ reflectivity grid index j
 * @param sunIndexI sun grid index i
 * @param sunIndexJ sun grid index j
 * @return angle between two grid index pairs
 */
    float gridAngle(int16_t loopI, int16_t loopJ, int16_t sunIndexI, int16_t sunIndexJ);
}

/**
 * Calculates the sunlight reflected off the Earth's surface
 * @param satellite satellite position in ECEF frame
 * @param sunPosition sun position in ECEF frame
 * @param reflectivityData Earth surface reflectivity data from TOMS project
 * @return sunlight reflected off the Earth's surface
 */
Matrix<float, ReflectivityDataRows, ReflectivityDataColumns>
calculateAlbedo(const Vector3f &satellite, const Vector3f &sunPosition,
                const Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> &reflectivityData);


#endif //ADCS_ONBOARD_SOFTWARE_ALBEDO_H
