#ifndef ADCS_ONBOARD_SOFTWARE_BDOT_H
#define ADCS_ONBOARD_SOFTWARE_BDOT_H

#include "Eigen/Geometry"

/**
 * Class that implements the Bdot controller used during the Detumbling Mode
 */
class Bdot {
private:
    /**
     * magnetic field value expressed in the Body Frame
     */
    Eigen::Vector3f magneticFieldBody;

    Eigen::Vector3f bDotVector;

    /**
     * proportional positive scalar gain
     */
    Eigen::Matrix<float, 3, 3> Kp;

    /**
     * Maximum magnetic dipole of magnetorquers (in Am^2)
     */
    Eigen::Vector3f maxMagneticDipole;

    /**
     * derivation value equal to the time-period elapsed between two measurements of the magnetic field (in sec)
     */
    float timestep;

public:

    /**
     * Class constructor
     * @param magneticFieldBody magnetic field value expressed in the Body Frame
     * @param Kp proportional positive scalar gain
     * @param maxMagneticDipole magnetorquers maximum magnetic dipole (in Am^2)
     * @param timestep derivation value (in sec)
     */
    Bdot(Eigen::Vector3f magneticFieldBody, Eigen::Matrix<float, 3, 3> Kp, Eigen::Vector3f maxMagneticDipole, float timestep);

    /**
     * Bdot controller is used in Detumbling mode, during which actuation is performed by the magnetorquers, and
     * utilizes only magnetometer data.
     * It calculates a magnetic dipole opposite to the derivative of the magnetic field.
     * @param magneticFieldBody magnetic field value expressed in the Body Frame
     * @return magnetic dipole value
     */
    Eigen::Vector3f controller(Eigen::Vector3f &magneticFieldBody);

    /**
     * Scaling of the desired magnetic dipole in case it exceeds the maximum dipole provided by each magnetorquer
     * @param magneticDipole desired magnetic dipole (in Am^2)
     * @return scaled magnetic dipole (in Am^2)
     */
    Eigen::Vector3f magnetorquerScaling(Eigen::Vector3f magneticDipole);

    /**
     * magnetic field value expressed in the Body Frame getter
     * @return magnetic field value expressed in the Body Frame
     */
    Eigen::Vector3f getMagneticFieldBody(){
        return magneticFieldBody;
    }

    /**
     * magnetic field value expressed in the Body Frame setter
     * @param magneticFieldBody magnetic field value expressed in the Body Frame
     */
    void setMagneticFieldBody(Eigen::Vector3f magneticFieldBody){
        this->magneticFieldBody = magneticFieldBody;
    }

    /**
     * Bdot vector getter
     * @return Bdot vector
     */
    Eigen::Vector3f getBDotVector(){
        return bDotVector;
    }

    /**
     * Bdot vector setter
     * @param bDotVector Bdot vector
     */
    void setBDotVector(Eigen::Vector3f bDotVector){
        this->bDotVector = bDotVector;
    }

    /**
     * proportional positive scalar gain getter
     * @return proportional positive scalar gain
     */
    Eigen::Matrix<float, 3, 3> getKp(){
        return Kp;
    }

    /**
     * proportional positive scalar gain setter
     * @param Kp proportional positive scalar gain
     */
    void setKp(Eigen::Matrix<float, 3, 3> Kp){
        this->Kp = Kp;
    }

    /**
     * Maximum magnetic dipole of magnetorquers (in Am^2) getter
     * @return Maximum magnetic dipole of magnetorquers (in Am^2)
     */
    Eigen::Vector3f getMaxMagneticDipole(){
        return maxMagneticDipole;
    }

    /**
     * Maximum magnetic dipole of magnetorquers (in Am^2) setter
     * @param maxMagneticDipole Maximum magnetic dipole of magnetorquers (in Am^2)
     */
    void setMaxMagneticDipole(Eigen::Vector3f maxMagneticDipole){
        this->maxMagneticDipole = maxMagneticDipole;
    }

    /**
     * derivation value getter
     * @return derivation value
     */
    float getTimestep(){
        return timestep;
    }

    /**
     * derivation value setter
     * @param timestep derivation value
     */
    void setTimestep(float timestep){
        this->timestep = timestep;
    }
};

#endif //ADCS_ONBOARD_SOFTWARE_BDOT_H