#pragma once

#include "Eigen/Geometry"

/**
 * Class that implements the Bdot controller used during the Detumbling Mode
 */
class Bdot {
private:
    /**
     * magnetic field values sampled consecutively at the beginning of the sampling period during a single control cycle,
     * expressed in the Body Frame.
     * The time difference between the two samples is equal to the Timestep value defined in bdot namespace
     */
    Eigen::Vector3f samplingBeginMagneticFieldBody;

    Eigen::Vector3f samplingEndMagneticFieldBody;

    Eigen::Vector3f bDotVector;
public:

    /**
     * @param samplingBeginMagneticFieldBody magnetic field value sampled at the beginning of the sampling period during
     * a single control cycle, expressed in the Body Frame.
     * @param samplingEndMagneticFieldBody second magnetic field value sampled consecutively after the first, at the beginning of the sampling period
     * during a single control cycle, expressed in the Body Frame.
     */
    Bdot(Eigen::Vector3f samplingBeginMagneticFieldBody, Eigen::Vector3f samplingEndMagneticFieldBody);

    /**
     * Bdot controller is used in Detumbling mode, during which actuation is performed by the magnetorquers, and
     * utilizes only magnetometer data.
     * It calculates a magnetic dipole opposite to the derivative of the magnetic field.
     * @return magnetic dipole value
     */
    Eigen::Vector3f controller();

    /**
     * Scaling of the desired magnetic dipole in case it exceeds the maximum dipole provided by each magnetorquer
     * @param magneticDipole desired magnetic dipole (in Am^2)
     * @return scaled magnetic dipole (in Am^2)
     */
    Eigen::Vector3f magnetorquerScaling(Eigen::Vector3f magneticDipole);

    /**
     * magnetic field value sampled at the beginning of the sampling period during a single control cycle,
     * expressed in the Body Frame getter
     * @return magnetic field value expressed in the Body Frame
     */
    Eigen::Vector3f getSamplingBeginMagneticFieldBody() const {
        return samplingBeginMagneticFieldBody;
    }

    /**
     * magnetic field value sampled at the beginning of the sampling period during a single control cycle,
     * expressed in the Body Frame setter
     * @param magneticFieldBody magnetic field value expressed in the Body Frame
     */
    void setSamplingBeginMagneticFieldBody(const Eigen::Vector3f samplingBeginMagneticFieldBody) {
        this->samplingBeginMagneticFieldBody = samplingBeginMagneticFieldBody;
    }

    /**
    * second magnetic field value sampled consecutively after the first, at the beginning of the sampling period
    * during a single control cycle, expressed in the Body Frame getter
    * @return magnetic field value expressed in the Body Frame
    */
    Eigen::Vector3f getSamplingEndMagneticFieldBody() const {
        return samplingEndMagneticFieldBody;
    }

    /**
     * second magnetic field value sampled consecutively after the first, at the beginning of the sampling period
     * during a single control cycle, expressed in the Body Frame setter
     * @param magneticFieldBody magnetic field value expressed in the Body Frame
     */
    void setSamplingEndMagneticFieldBody(const Eigen::Vector3f samplingEndMagneticFieldBody) {
        this->samplingEndMagneticFieldBody = samplingEndMagneticFieldBody;
    }

    /**
     * Bdot vector getter
     * @return Bdot vector
     */
    Eigen::Vector3f getBDotVector() const {
        return bDotVector;
    }

    /**
     * Bdot vector setter
     * @param bDotVector Bdot vector
     */
    void setBDotVector(const Eigen::Vector3f bDotVector) {
        this->bDotVector = bDotVector;
    }
};
