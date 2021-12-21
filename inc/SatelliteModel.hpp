#pragma once 

#include "Eigen/Dense"
#include "Definitions.hpp"

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
     * @param satPositionECI satellite position expressed in ECI frame
     * @param albedo the fraction of the sunlight diffused from earth that interacts with the satellite
     * @return magnetic field and sun position measurements in body frame
     */
    MeasurementVector
    measurementFunction(Eigen::Vector3f magneticField,
                        Eigen::Vector3f sunPosition, bool eclipse,
                        GlobalStateVector state, Eigen::Vector3f satPositionECI, float albedo) const;

    /**
     * Updates the state vector taking into account measurements from gyroscope
     * @param state 7-element vector consisting of the quaternion representation of the satellite's orientation and bias
     * @param gyroMeasurements measurements from gyroscope
     * @return updated state
     */
    GlobalStateVector stateTransitionFunction(GlobalStateVector state,
                                              Eigen::Vector3f gyroMeasurements) const;

    /**
     * Calculates the state's Jacobian
     * @param state 7-element vector consisting of the quaternion representation of the satellite's orientation and bias
     * @param gyroMeasurements measurements from gyroscope
     * @return state's Jacobian
     */
    Eigen::Matrix<float, LocalStateSize, LocalStateSize>
    stateTransitionJacobian(GlobalStateVector state,
                            Eigen::Vector3f gyroMeasurements) const;

    /**
     * Calculates the measurements' Jacobian
     * @param magneticField magnetic field measurements
     * @param sunPosition sun position measurements
     * @param eclipse existence or not of eclipse
     * @param state 7-element vector consisting of the quaternion representation of the satellite's orientation and bias
     * @param albedo the sunlight reflected off the Earth's surface
     * @return measurements' Jacobian
     */
    Eigen::Matrix<float, MeasurementSize, MeasurementSize>
    measurementJacobian(Eigen::Vector3f magneticField,
                        Eigen::Vector3f sunPosition, bool eclipse,
                        GlobalStateVector state, Eigen::Vector3f satPositionECI, float albedo) const;
};