#ifndef ADCS_ONBOARD_SOFTWARE_ALBEDO_H
#define ADCS_ONBOARD_SOFTWARE_ALBEDO_H

#include "Eigen/Geometry"

using namespace Eigen;

const int16_t ReflectivityDataRows = 180;
const int16_t ReflectivityDataColumns = 288;

const static float EMR = 6371.01e3;

class Albedo {
private:

    Vector<float, ReflectivityDataRows> phis;
    Vector<float, ReflectivityDataColumns> thetas;
    Vector<float, ReflectivityDataRows> cellAreas;

public:
    Albedo();
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
    Vector2f indicesToRadians(int16_t i, int16_t j);

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

    Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> earthFieldOfView(Vector3f satelliteSpherical);


/**
 * Calculates the sunlight reflected off the Earth's surface
 * @param satellite satellite position in ECEF frame
 * @param sunPosition sun position in ECEF frame
 * @param reflectivityData Earth surface reflectivity data from TOMS project
 * @return sunlight reflected off the Earth's surface
 */
    float
    calculateAlbedo(const Vector3f &satellite, const Vector3f &sunPosition,
                    const Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> &reflectivityData);

};
#endif //ADCS_ONBOARD_SOFTWARE_ALBEDO_H
