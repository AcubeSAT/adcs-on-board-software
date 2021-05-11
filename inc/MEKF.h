
#ifndef ADCS_ONBOARD_SOFTWARE_MEKF_H
#define ADCS_ONBOARD_SOFTWARE_MEKF_H

#include "Eigen/Dense"
#include "SatelliteModel.h"
#include "unsupported/Eigen/MatrixFunctions"

/**
 * Class that implements the discrete Multiplicative Extended Kalman Filter algorithm.
 */
class MEKF {
private:
    /**
     * state transition function Jacobian matrix
     */
    Matrix<float, localStateSize, localStateSize> F_k;

    /**
     * measurement function Jacobian matrix
     */
    Matrix<float, measurementSize, localStateSize> H_k;

    /**
     * Kalman gain
     */
    Matrix<float, localStateSize, measurementSize> K;

    /**
     * process noise covariance matrix
     */
    Matrix<float, localStateSize, localStateSize> Q;

    /**
     * measurement noise covariance matrix
     */
    Matrix<float, measurementSize, measurementSize> R;

    /**
     * estimation error covariance matrix
     */
    Matrix<float, localStateSize, localStateSize> P = Matrix<float, localStateSize, localStateSize>::Identity();

    /**
     * state of the system, consisting the quaternion from ECI to Body frame and the bias of the gyroscope
     */
    GlobalStateVector globalState{1, 0, 0, 0, 0, 0, 0};

public:

    /**
     * Class Constructor that initialises Q, R, P and globalState
     * @param Q process noise covariance matrix
     * @param R measurement noise covariance matrix
     */
    MEKF(const Matrix<float, localStateSize, localStateSize> &Q,
         const Matrix<float, localStateSize, localStateSize> &R)
            : Q{Q}, R{R} {
    }

    /**
     * Q Setter
     * @param Q process noise covariance matrix
     */
    void setQ(const Matrix<float, localStateSize, localStateSize> &Q) {
        this->Q = Q;
    }

    /**
     * Q Getter
     * @return Q process noise covariance matrix
     */
    Matrix<float, localStateSize, localStateSize> getQ() const {
        return Q;
    }

    /**
     * R Setter
     * @param R measurement noise covariance matrix
     */
    void setR(const Matrix<float, localStateSize, localStateSize> &R) {
        this->R = R;
    }

    /**
     * R Getter
     * @return R measurement noise covariance matrix
     */
    Matrix<float, measurementSize, measurementSize> getR() const {
        return R;
    }

    /**
     * P Setter
     * @param P estimation error covariance matrix
     */
    void setP(const Matrix<float, localStateSize, localStateSize> &P) {
        this->P = P;
    }

    /**
     * P Getter
     * @return P estimation error covariance matrix
     */
    Matrix<float, localStateSize, localStateSize> getP() const {
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
    void predict(float timestep, SatelliteModel satelliteModel, const Vector3f &gyroMeasurements);

    /**
     * MEKF Correction. A modified version of the classic Extended Kalman Filter version that uses a local error state to update the quaternion and the bias.
     * @param measurement vector with the measurements of magnetic field vector and sun vector in the first frame, in our case Body
     * @param magneticField magnetic field vector in the second frame, in our case ECI
     * @param sunPosition sun vector in the second frame, in our case ECI
     * @param eclipse bool variable which is set to false if during this timestep the satellite is not in eclipse
     * @param satelliteModel object of the Class SatelliteModel that implements the space environment
     */
    void correct(const MeasurementVector &measurement, const Vector3f &magneticField,
                 const Vector3f &sunPosition, bool eclipse, SatelliteModel satelliteModel);
};


#endif //ADCS_ONBOARD_SOFTWARE_MEKF_H
