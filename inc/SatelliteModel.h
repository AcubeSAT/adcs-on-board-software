#ifndef MAIN_SATELLITEMODEL_H
#define MAIN_SATELLITEMODEL_H


#include "Eigen/Dense"

using namespace Eigen;

const uint8_t localStateSize = 6;
const uint8_t measurementSize = 6;

typedef Vector<float, measurementSize> MeasurementVector;
typedef Vector<float, 7> GlobalStateVector;


/**
 * Class that implements the space environment
 */
class SatelliteModel {
private:
    /**
     * timestep sampling time in seconds
     */
    float timestep;

public:
    /**
     * Class constructor that initializes the timestep
     * @param timestep sampling time in seconds
     */
    SatelliteModel(float timestep);

    /**
     * Rotates the magnetic field and sun position measurements to body frame
     * @param magneticField magnetic field measurements
     * @param sunPosition sun position measurements
     * @param eclipse existence or not of eclipse
     * @param state 7-element vector consisting of the quaternion representation of the satellite's orientation and bias
     * @return magnetic field and sun position measurements in body frame
     */
    MeasurementVector
    measurementFunction(Vector3f magneticField,
                        Vector3f sunPosition, bool eclipse,
                        GlobalStateVector state);

    /**
     * Updates the state vector taking into account measurements from gyroscope
     * @param state 7-element vector consisting of the quaternion representation of the satellite's orientation and bias
     * @param gyroMeasurements measurements from gyroscope
     * @return updated state
     */
    GlobalStateVector stateTransitionFunction(GlobalStateVector state,
                                              Vector3f gyroMeasurements);

    /**
     * Calculates the state's Jacobian
     * @param state 7-element vector consisting of the quaternion representation of the satellite's orientation and bias
     * @param gyroMeasurements measurements from gyroscope
     * @return state's Jacobian
     */
    Matrix<float, localStateSize, localStateSize>
    stateTransitionJacobian(GlobalStateVector state,
                            Vector3f gyroMeasurements);

    /**
     * Calculates the measurements' Jacobian
     * @param magneticField magnetic field measurements
     * @param sunPosition sun position measurements
     * @param eclipse existence or not of eclipse
     * @param state 7-element vector consisting of the quaternion representation of the satellite's orientation and bias
     * @return measurements' Jacobian
     */
    Matrix<float, measurementSize, measurementSize>
    measurementJacobian(Vector3f magneticField,
                        Vector3f sunPosition, bool eclipse,
                        GlobalStateVector state);
};


#endif //MAIN_SATELLITEMODEL_H