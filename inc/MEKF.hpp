
#ifndef ADCS_ONBOARD_SOFTWARE_MEKF_H
#define ADCS_ONBOARD_SOFTWARE_MEKF_H

#include "Eigen/Dense"
#include "SatelliteModel.hpp"
#include "unsupported/Eigen/MatrixFunctions"

/**
 * Class that implements the discrete Multiplicative Extended Kalman Filter algorithm.
 */
class MEKF {
private:
    /**
     * state transition function Jacobian Matrix
     */
    Eigen::Matrix<float, LocalStateSize, LocalStateSize> F_k;

    /**
     * measurement function Jacobian Matrix
     */
    Eigen::Matrix<float, MeasurementSize, LocalStateSize> H_k;

    /**
     * Kalman gain
     */
    Eigen::Matrix<float, LocalStateSize, MeasurementSize> K;

    /**
     * process noise covariance Matrix
     */
    Eigen::Matrix<float, LocalStateSize, LocalStateSize> Q;

    /**
     * measurement noise covariance Matrix
     */
    Eigen::Matrix<float, MeasurementSize, MeasurementSize> R;

    /**
     * estimation error covariance Matrix
     */
    Eigen::Matrix<float, LocalStateSize, LocalStateSize> P = Eigen::Matrix<float, LocalStateSize, LocalStateSize>::Identity();

    /**
     * state of the system, consisting the quaternion from ECI to Body frame and the bias of the gyroscope
     */
    GlobalStateVector globalState{1, 0, 0, 0, 0, 0, 0};

public:

    /**
     * Class Constructor that initialises Q, R, P and globalState
     * @param Q process noise covariance Matrix
     * @param R measurement noise covariance Matrix
     */
    MEKF(const Eigen::Matrix<float, LocalStateSize, LocalStateSize> &Q,
         const Eigen::Matrix<float, LocalStateSize, LocalStateSize> &R)
            : Q{Q}, R{R} {
    }

    /**
     * Q Setter
     * @param Q process noise covariance Matrix
     */
    void setQ(const Eigen::Matrix<float, LocalStateSize, LocalStateSize> &Q) {
        this->Q = Q;
    }

    /**
     * Q Getter
     * @return Q process noise covariance Matrix
     */
    Eigen::Matrix<float, LocalStateSize, LocalStateSize> getQ() const {
        return Q;
    }

    /**
     * R Setter
     * @param R measurement noise covariance Matrix
     */
    void setR(const Eigen::Matrix<float, LocalStateSize, LocalStateSize> &R) {
        this->R = R;
    }

    /**
     * R Getter
     * @return R measurement noise covariance Matrix
     */
    Eigen::Matrix<float, MeasurementSize, MeasurementSize> getR() const {
        return R;
    }

    /**
     * P Setter
     * @param P estimation error covariance Matrix
     */
    void setP(const Eigen::Matrix<float, LocalStateSize, LocalStateSize> &P) {
        this->P = P;
    }

    /**
     * P Getter
     * @return P estimation error covariance Matrix
     */
    Eigen::Matrix<float, LocalStateSize, LocalStateSize> getP() const {
        return P;
    }

    /**
     * globalState Setter
     * @param globalState state of the system
     */
    void setGlobalState(const GlobalStateVector &globalState) {
        this->globalState = globalState;
    }

    /**
     * globalState Getter
     * @return globalState state of the system
     */
    GlobalStateVector getGlobalState() const {
        return globalState;
    }

    /**
     * MEKF prediction.
     * @param timestep sampling time in seconds
     * @param satelliteModel object of the Class SatelliteModel that implements the space environment
     * @param gyroMeasurements measurements of the gyroscope in this timestep
     */
    void predict(float timestep, const SatelliteModel &satelliteModel, const Eigen::Vector3f &gyroMeasurements);

    /**
     * MEKF Correction. A modified version of the classic Extended Kalman Filter version that uses a local error state to update the quaternion and the bias.
     * @param measurement vector with the measurements of magnetic field vector and sun vector in the first frame, in our case Body
     * @param magneticField magnetic field vector in the second frame, in our case ECI
     * @param sunPosition sun vector in the second frame, in our case ECI
     * @param eclipse bool variable which is set to false if during this timestep the satellite is not in eclipse
     * @param satelliteModel object of the Class SatelliteModel that implements the space environment
     * @param satellitePositionECI satellite position in ECI frame
     * @param albedo the fraction of the sunlight reflected off the Earth's surface
     */
    void correct(const MeasurementVector &measurement, const Eigen::Vector3f &magneticField,
                 const Eigen::Vector3f &sunPosition, bool eclipse, const SatelliteModel &satelliteModel, Eigen::Vector3f satellitePositionECI,
                 float albedo);
};


#endif //ADCS_ONBOARD_SOFTWARE_MEKF_H
