#pragma once 

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
<<<<<<< HEAD

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

=======
>>>>>>> master
public:

    /**
     * @param magneticFieldBody magnetic field value expressed in the Body Frame
     */
    Bdot(Eigen::Vector3f magneticFieldBody);

    /**
     * Bdot controller is used in Detumbling mode, during which actuation is performed by the magnetorquers, and
     * utilizes only magnetometer data.
     * It calculates a magnetic dipole opposite to the derivative of the magnetic field.
     * @param magneticFieldBody magnetic field value expressed in the Body Frame
     * @return magnetic dipole value
     */
    Eigen::Vector3f controller(const Eigen::Vector3f &magneticFieldBody);

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
    Eigen::Vector3f getMagneticFieldBody() const{
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
    Eigen::Vector3f getBDotVector() const{
        return bDotVector;
    }

    /**
     * Bdot vector setter
     * @param bDotVector Bdot vector
     */
    void setBDotVector(Eigen::Vector3f bDotVector){
        this->bDotVector = bDotVector;
    }
};
