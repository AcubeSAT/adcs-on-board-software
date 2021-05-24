#ifndef ADCS_ONBOARD_SOFTWARE_ALBEDO_H
#define ADCS_ONBOARD_SOFTWARE_ALBEDO_H

#include "Eigen/Geometry"

using namespace Eigen;

const uint8_t reflectivityDataRows = 4;
const uint16_t reflectivityDataColumns = 5;

class Albedo {

public:
    Vector<int8_t, 2> radiansToIndices(float theta, float phi);

    Vector2f indicesToRadians(int8_t i, int8_t j);

    float calculateCellArea(int8_t i, int8_t j);

    float gridAngle(int8_t loopI, int8_t loopJ, int8_t sunIndexI, int8_t sunIndexJ);

    Matrix<float, reflectivityDataRows, reflectivityDataColumns>
    calculateAlbedo(Vector3f satellite, Vector3f sunPosition,
                    Matrix<float, reflectivityDataRows, reflectivityDataColumns> reflectivityData);

};

#endif //ADCS_ONBOARD_SOFTWARE_ALBEDO_H
