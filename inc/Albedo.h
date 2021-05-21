#ifndef ADCS_ONBOARD_SOFTWARE_ALBEDO_H
#define ADCS_ONBOARD_SOFTWARE_ALBEDO_H

const uint8_t reflectivityDataRows = 180;
const uint16_t reflectivityDataColumns = 288;

class Albedo {

    Vector<float, 2> radiansToIndices(Vector3f vectorSpherical);

    Vector<float, 2> indicesToRadians(uint8_t i, uint8_t j) ;

    float calculateCellArea(uint8_t i, uint8_t j);

    float gridAngle(uint8_t loopI, uint8_t loopJ, uint8_t sunIndexI, uint8_t sunIndexJ);

    Vector3f albedo(Vector3f satellite, Vector3f sunPosition,
                    Matrix<float, reflectivityDataRows, reflectivityDataColumns> reflectivityData);

};


#endif //ADCS_ONBOARD_SOFTWARE_ALBEDO_H
