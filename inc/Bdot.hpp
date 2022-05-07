#pragma once

#include "Eigen/Geometry"

/**
 * Class that implements the Bdot controller used during the Detumbling Mode
 */
class Bdot {
private:
    /**
     * magnetic field values at the beginning and at the end of every control cycle, expressed in the Body Frame.
     * The time difference between sampling the two values is equal to the Timestep value defined in bdot namespace
     */
    Eigen::Vector3f cycleBeginMagneticFieldBody;

    Eigen::Vector3f cycleEndMagneticFieldBody;

    Eigen::Vector3f bDotVector;
public:

    /**
     * @param cycleBeginMagneticFieldBody magnetic field value at the beginning of a control cycle expressed in the Body Frame
     * @param cycleEndMagneticFieldBody magnetic field value at the end of a control cycle expressed in the Body Frame
     */
    Bdot(Eigen::Vector3f cycleBeginMagneticFieldBody, Eigen::Vector3f cycleEndMagneticFieldBody);

    /**
     * Bdot controller is used in Detumbling mode, during which actuation is performed by the magnetorquers, and
     * utilizes only magnetometer data.
     * It calculates a magnetic dipole opposite to the derivative of the magnetic field.
     * @param cycleBeginMagneticFieldBody magnetic field value at the beginning of a control cycle expressed in the Body Frame
     * @param cycleEndMagneticFieldBody magnetic field value at the end of a control cycle expressed in the Body Frame
     * @return magnetic dipole value
     */
    Eigen::Vector3f
    controller(const Eigen::Vector3f &cycleBeginMagneticFieldBody, const Eigen::Vector3f &cycleEndMagneticFieldBody);

    /**
     * Scaling of the desired magnetic dipole in case it exceeds the maximum dipole provided by each magnetorquer
     * @param magneticDipole desired magnetic dipole (in Am^2)
     * @return scaled magnetic dipole (in Am^2)
     */
    Eigen::Vector3f magnetorquerScaling(Eigen::Vector3f magneticDipole);

    /**
     * magnetic field value at the beginning of a control cycle expressed in the Body Frame getter
     * @return magnetic field value expressed in the Body Frame
     */
    Eigen::Vector3f getCycleBeginMagneticFieldBody() const {
        return cycleBeginMagneticFieldBody;
    }

    /**
     * magnetic field value at the beginning of a control cycle expressed in the Body Frame setter
     * @param magneticFieldBody magnetic field value expressed in the Body Frame
     */
    void setCycleBeginMagneticFieldBody(Eigen::Vector3f cycleBeginMagneticFieldBody) {
        this->cycleBeginMagneticFieldBody = cycleBeginMagneticFieldBody;
    }

    /**
    * magnetic field value at the end of a control cycle expressed in the Body Frame getter
    * @return magnetic field value expressed in the Body Frame
    */
    Eigen::Vector3f getCycleEndMagneticFieldBody() const {
        return cycleEndMagneticFieldBody;
    }

    /**
     * magnetic field value at the end of a control cycle expressed in the Body Frame setter
     * @param magneticFieldBody magnetic field value expressed in the Body Frame
     */
    void setCycleEndMagneticFieldBody(Eigen::Vector3f cycleEndMagneticFieldBody) {
        this->cycleEndMagneticFieldBody = cycleEndMagneticFieldBody;
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
    void setBDotVector(Eigen::Vector3f bDotVector) {
        this->bDotVector = bDotVector;
    }
};
