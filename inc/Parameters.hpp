#pragma once

#include "Eigen/Geometry"
#include "Definitions.hpp"

/**
 * Groups all the parameters that remain constant during flight and will change only in emergency circumstances (e.g. in-flight gain tuning) 
 */
namespace Parameters {
    /**
     * Estimation of the residual magnetic dipole (in Am^2)
     */
    inline const Eigen::Vector3f ResidualDipoleEstimation = {0.05, 0.05, 0.05};

    namespace Actuators {
        /**
         * Maximum magnetic dipole provided by the Magnetorquers (in Am^2)
         */
        inline const Eigen::Vector3f MaxMagneticDipole = {0.2, 0.2, 0.2};

        /**
         * Reaction Wheel Maximum Rotation Rate (in rpm)
         */
        inline const float ReactionWheelAngularVelocityLimit = 10000;

        /**
         * Percentage of RW desaturation torque added to the torque given on Z-axis through magnetorquers
         */
        inline const float TorquePercentage = 0.12;

        /**
         * (in kg*m^2)
         */
        inline const float FlywheelInertia = 0.0015;

        /**
         * Maximum torque provided by the Reaction Wheel (in Nm)
         */
        inline const float MaxReactionWheelTorque = 1e-04;
    }

    namespace BDot {
        /**
         * Proportional positive scalar gain
         */
        inline const auto Kp = Eigen::Matrix<float, VectorSize, VectorSize>::Identity(
                VectorSize, VectorSize);

        /**
         * Derivation value equal to the time-period elapsed between two measurements of the magnetic field (in sec)  
         */
        inline const float Timestep = 0.1;
    }

    namespace Detumbling {
        /**
         * The inertia matrix of the whole AcubeSAT nanosatellite
         */
        inline const auto Inertia = Eigen::Matrix<float, 3, 3>({
                                                                       {0.0412158223637031,    -1.85801942968305e-06, 0.00137245665256380},
                                                                       {-1.85801942968300e-06, 0.0410190161501943,    -1.64802792093441e-05},
                                                                       {0.00137245665256380,   -1.64802792093441e-05, 0.00695520282610255}
                                                               });

        /**
         * Update timestep
         */
        inline const auto Timestep = 0.1;

        /**
         * Duration of the actuation period during a single control cycle
         */
        inline const auto ActuationTime = 0.7;

        /**
         * The required angular velocity value on every axis in order to switch to Nominal mode
         */
        inline const auto AngularVelocityTriggerValue = 0.035;
    }

    namespace SatelliteModel {
        /**
         * Sampling time in seconds
         */
        inline const float Timestep = 0.1;
    }

    namespace CovarianceMatrices {
        /**
         * Process noise covariance Matrix
         */
        inline const Eigen::Matrix<float, LocalStateSize, LocalStateSize> Q{{1e-04, 0,     0,     0,     0,     0},
                                                                            {0,     1e-04, 0,     0,     0,     0},
                                                                            {0,     0,     1e-04, 0,     0,     0},
                                                                            {0,     0,     0,     1e-07, 0,     0},
                                                                            {0,     0,     0,     0,     1e-07, 0},
                                                                            {0,     0,     0,     0,     0,     1e-07}};

        /**
         * Measurement noise covariance Matrix
         */
        inline const Eigen::Matrix<float, MeasurementSize, MeasurementSize> R{{5e-05, 0,     0,     0,     0,     0},
                                                                              {0,     5e-05, 0,     0,     0,     0},
                                                                              {0,     0,     5e-05, 0,     0,     0},
                                                                              {0,     0,     0,     1e-04, 0,     0},
                                                                              {0,     0,     0,     0,     1e-04, 0},
                                                                              {0,     0,     0,     0,     0,     1e-04}};
    }

    namespace NadirPointingPlusRW {
        /**
         * Proportional positive gain
         */
        inline const auto Kp = Eigen::Matrix<float, VectorSize, VectorSize>::Identity(
                VectorSize, VectorSize);

        /**
         * Derivative positive gain
         */
        inline const auto Kd = Eigen::Matrix<float, VectorSize, VectorSize>::Identity(
                VectorSize, VectorSize);
    }

    namespace NadirPointingMagnetorquerOnly {
        /**
         * Proportional positive gain
         */
        inline const auto Kp = Eigen::Matrix<float, VectorSize, VectorSize>::Identity(
                VectorSize, VectorSize);

        /**
         * Derivative positive gain
         */
        inline const auto Kd = Eigen::Matrix<float, VectorSize, VectorSize>::Identity(
                VectorSize, VectorSize);
    }

    namespace SunPointingPlusRW {
        /**
         * Proportional positive gain
         */
        inline const auto Kp = Eigen::Matrix<float, VectorSize, VectorSize>::Identity(
                VectorSize, VectorSize);

        /**
         * Derivative positive gain
         */
        inline const auto Kd = Eigen::Matrix<float, VectorSize, VectorSize>::Identity(
                VectorSize, VectorSize);
    }

    namespace SunPointingMagnetorquerOnly {
        /**
         * Proportional positive gain
         */
        inline const auto Kp = Eigen::Matrix<float, VectorSize, VectorSize>::Identity(
                VectorSize, VectorSize);

        /**
         * Derivative positive gain
         */
        inline const auto Kd = Eigen::Matrix<float, VectorSize, VectorSize>::Identity(
                VectorSize, VectorSize);
    }

    /** 
     * Desired quaternion to achieve pointing
     */
    inline const Eigen::Quaternionf DesiredQuaternion = {1, 0, 0, 0};

    /** 
     * Angular velocity of the orbit frame with respect to the ECI frame, expressed in the orbit frame
     */
    inline const Eigen::Vector3f AngularVelocityECIOrbit = {0, 0.0011, 0};
}
