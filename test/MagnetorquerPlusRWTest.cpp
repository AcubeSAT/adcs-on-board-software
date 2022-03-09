#include <catch2/catch.hpp>
#include "MagnetorquerPlusRW.hpp"

using namespace Eigen;


const MagnetorquerPlusRW magnetorquerPlusRW;

TEST_CASE("Magnetorquer Plus RW Strategy - Torque Split") {
    Vector3f commandedTorque1 = {1e-06, 1e-06, 1e-06};
    Vector3f magneticField1 = {0, 0, 1e-05};

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque1 = magnetorquerPlusRW.splitTorque(magneticField1,
                                                                                               commandedTorque1);

    Vector3f desiredMagneticTorque1 = actuatorTorque1.col(0);
    Vector3f desiredReactionWheelTorque1 = actuatorTorque1.col(1);

    Vector3f commandedTorque2 = {-1e-03 * 0.40561, -1e-03 * 0.55451, 1e-03 * 0.29295};
    Vector3f magneticField2 = {1e-04 * 0.44848, -1e-04 * 0.02672, -1e-04 * 0.14255};
    Matrix<float, VectorSize, NumOfActuators> actuatorTorque2 = magnetorquerPlusRW.splitTorque(magneticField2,
                                                                                               commandedTorque2);

    Vector3f desiredMagneticTorque2 = actuatorTorque2.col(0);
    Vector3f desiredReactionWheelTorque2 = actuatorTorque2.col(1);

    Vector3f commandedTorque3 = {1e-04 * 0.52402, -1e-04 * 0.13596, 1e-04 * 0.72617};
    Vector3f magneticField3 = {1e-04 * 0.44672, -1e-04 * 0.03053, -1e-04 * 0.14552};
    Matrix<float, VectorSize, NumOfActuators> actuatorTorque3 = magnetorquerPlusRW.splitTorque(magneticField3,
                                                                                               commandedTorque3);

    Vector3f desiredMagneticTorque3 = actuatorTorque3.col(0);
    Vector3f desiredReactionWheelTorque3 = actuatorTorque3.col(1);

    REQUIRE(desiredMagneticTorque1(0) == Approx(1e-06).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque1(1) == Approx(1e-06).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque1(2) == Approx(0).epsilon(1e-4));

    REQUIRE(desiredReactionWheelTorque1(0) == Approx(0).epsilon(1e-4));
    REQUIRE(desiredReactionWheelTorque1(1) == Approx(0).epsilon(1e-4));
    REQUIRE(desiredReactionWheelTorque1(2) == Approx(1e-06).epsilon(1e-4));

    REQUIRE(desiredMagneticTorque2(0) == Approx(-0.00040561).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque2(1) == Approx(-0.00055451).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque2(2) == Approx(-0.00117214).epsilon(1e-4));

    REQUIRE(desiredReactionWheelTorque2(0) == Approx(0).epsilon(1e-4));
    REQUIRE(desiredReactionWheelTorque2(1) == Approx(0).epsilon(1e-4));
    REQUIRE(desiredReactionWheelTorque2(2) == Approx(0.001465).epsilon(1e-4));

    REQUIRE(desiredMagneticTorque3(0) == Approx(1e-03 * 0.05240).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque3(1) == Approx(-1e-03 * 0.0135967).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque3(2) == Approx(1e-03 * 0.16371).epsilon(1e-4));

    REQUIRE(desiredReactionWheelTorque3(0) == Approx(0).epsilon(1e-4));
    REQUIRE(desiredReactionWheelTorque3(1) == Approx(0).epsilon(1e-4));
    REQUIRE(desiredReactionWheelTorque3(2) == Approx(-1e-04 * 0.91095).epsilon(1e-4));
}

TEST_CASE("Magnetorquer Plus RW Strategy - Magnetorquer Saturation") {
    Vector3f commandedTorque1 = {5e-06, 5e-06, 0};
    Vector3f magneticField1 = {0, 0, 1e-05};
    Vector3f desiredMagneticDipole1 = {-0.5, 0.5, 0};
    Vector3f desiredReactionWheelTorque1 = {0, 0, 0};
    Vector3f desiredMagneticTorque1 = {5e-06, 5e-06, 0};

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque1 = magnetorquerPlusRW.desaturateMagnetorquer(
            desiredMagneticTorque1,
            desiredReactionWheelTorque1,
            commandedTorque1,
            magneticField1,
            desiredMagneticDipole1);
    desiredMagneticTorque1 = actuatorTorque1.col(0);
    desiredReactionWheelTorque1 = actuatorTorque1.col(1);

    Vector3f commandedTorque2 = {-1.0e-03 * 0.405619, -1.0e-03 * 0.554512, 1.0e-03 * 0.292957};
    Vector3f magneticField2 = {1.0e-04 * 0.448484, -1.0e-04 * 0.026721, -1.0e-04 * 0.142556};
    Vector3f desiredMagneticDipole2 = {-2.148192, 26.263678, -11.681327};
    Vector3f desiredReactionWheelTorque2 = {0, 0, 0.001465101};
    Vector3f desiredMagneticTorque2 = {-0.000405, -0.000554, -0.001172};

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque2 = magnetorquerPlusRW.desaturateMagnetorquer(
            desiredMagneticTorque2,
            desiredReactionWheelTorque2,
            commandedTorque2,
            magneticField2,
            desiredMagneticDipole2);
    desiredMagneticTorque2 = actuatorTorque2.col(0);
    desiredReactionWheelTorque2 = actuatorTorque2.col(1);

    REQUIRE(desiredMagneticTorque1(0) == Approx(1.5e-06).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque1(1) == Approx(1.5e-06).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque1(2) == Approx(0).epsilon(1e-4));

    REQUIRE(desiredReactionWheelTorque1(0) == Approx(0).epsilon(1e-4));
    REQUIRE(desiredReactionWheelTorque1(1) == Approx(0).epsilon(1e-4));
    REQUIRE(desiredReactionWheelTorque1(2) == Approx(0).epsilon(1e-4));

    REQUIRE(desiredMagneticTorque2(0) == Approx(-1e-04 * 0.038610).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque2(1) == Approx(-1e-04 * 0.0527832).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque2(2) == Approx(-1e-04 * 0.111575).epsilon(1e-4));

    REQUIRE(desiredReactionWheelTorque2(0) == Approx(0).epsilon(1e-4));
    REQUIRE(desiredReactionWheelTorque2(1) == Approx(0).epsilon(1e-4));
    REQUIRE(desiredReactionWheelTorque2(2) == Approx(1e-04 * 0.139415).epsilon(1e-4));

    SECTION("Zero desired dipole in all axes") {
        Vector3f commandedTorque = {5e-06, 5e-06, 0};
        Vector3f magneticField = {0, 0, 1e-05};
        Vector3f desiredMagneticDipole = {0, 0, 0};
        Vector3f desiredReactionWheelTorque = {0, 0, 0.001465101};
        Vector3f desiredMagneticTorque = {-0.000405, -0.000554, -0.001172};

        Matrix<float, VectorSize, NumOfActuators> actuatorTorque = magnetorquerPlusRW.desaturateMagnetorquer(
                desiredMagneticTorque,
                desiredReactionWheelTorque,
                commandedTorque,
                magneticField,
                desiredMagneticDipole);
        desiredMagneticTorque = actuatorTorque.col(0);
        desiredReactionWheelTorque = actuatorTorque.col(1);

        REQUIRE(desiredMagneticTorque(0) == Approx(1e-04 * 0.041236).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(1) == Approx(1e-04 * 0.0564068).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(2) == Approx(1e-04 * 0.11933).epsilon(1e-4));

        REQUIRE(desiredReactionWheelTorque(0) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredReactionWheelTorque(1) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredReactionWheelTorque(2) == Approx(-1e-04 * 0.11933).epsilon(1e-4));

    }

    SECTION("Zero desired dipole in two axes - Division by zero") {
        Vector3f commandedTorque = {5e-06, 5e-06, 0};
        Vector3f magneticField = {0, 0, 1e-05};
        Vector3f desiredMagneticDipole = {0, 0, 0.5};
        Vector3f desiredReactionWheelTorque = {0, 0, 0.001465101};
        Vector3f desiredMagneticTorque = {-0.000405, -0.000554, -0.001172};

        Matrix<float, VectorSize, NumOfActuators> actuatorTorque = magnetorquerPlusRW.desaturateMagnetorquer(
                desiredMagneticTorque,
                desiredReactionWheelTorque,
                commandedTorque,
                magneticField,
                desiredMagneticDipole);
        desiredMagneticTorque = actuatorTorque.col(0);
        desiredReactionWheelTorque = actuatorTorque.col(1);

        REQUIRE(desiredMagneticTorque(0) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(1) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(2) == Approx(0).epsilon(1e-4));

        REQUIRE(desiredReactionWheelTorque(0) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredReactionWheelTorque(1) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredReactionWheelTorque(2) == Approx(1e-04).epsilon(1e-4));
    }

    SECTION("Zero desired dipole in one axis - Division by zero") {
        Vector3f commandedTorque = {5e-06, 5e-06, 0};
        Vector3f magneticField = {0, 0, 1e-05};
        Vector3f desiredMagneticDipole = {0, 0.5, 0.5};
        Vector3f desiredReactionWheelTorque = {0, 0, 0.001465101};
        Vector3f desiredMagneticTorque = {-0.000405, -0.000554, -0.001172};

        Matrix<float, VectorSize, NumOfActuators> actuatorTorque = magnetorquerPlusRW.desaturateMagnetorquer(
                desiredMagneticTorque,
                desiredReactionWheelTorque,
                commandedTorque,
                magneticField,
                desiredMagneticDipole);
        desiredMagneticTorque = actuatorTorque.col(0);
        desiredReactionWheelTorque = actuatorTorque.col(1);

        REQUIRE(desiredMagneticTorque(0) == Approx(2.5e-06).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(1) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(2) == Approx(0).epsilon(1e-4));

        REQUIRE(desiredReactionWheelTorque(0) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredReactionWheelTorque(1) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredReactionWheelTorque(2) == Approx(1e-04).epsilon(1e-4));

    }
}

TEST_CASE("Magnetorquer Plus RW Strategy - Reaction Wheel Saturation") {
    Vector3f effectiveMagneticTorque1 = {2e-06, -1e-06, 1e-06};
    Vector3f reactionWheelTorque1 = {0, 0, 1e-05};
    Vector3f magneticFieldBody1 = {0, 0, 1e-05};
    float reactionWheelAngularVelocity1 = 11000;
    float reactionWheelAngularAcceleration1 = 10;

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque1 = magnetorquerPlusRW.desaturateReactionWheel(
            effectiveMagneticTorque1, reactionWheelTorque1, magneticFieldBody1,
            reactionWheelAngularVelocity1, reactionWheelAngularAcceleration1);

    effectiveMagneticTorque1 = actuatorTorque1.col(0);
    reactionWheelTorque1 = actuatorTorque1.col(1);

    Vector3f effectiveMagneticTorque2 = {-1.0e-05 * 0.32906, -1.0e-05 * 0.80912, -1.0e-05 * 0.87793};
    Vector3f reactionWheelTorque2 = {0, 0, 1e-04 * 0.140018};
    Vector3f magneticFieldBody2 = {1.0e-04 * 0.44848, -1.0e-04 * 0.02672, -1.0e-04 * 0.14255};
    float reactionWheelAngularVelocity2 = 0;
    float reactionWheelAngularAcceleration2 = 0;

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque2 = magnetorquerPlusRW.desaturateReactionWheel(
            effectiveMagneticTorque2, reactionWheelTorque2, magneticFieldBody2,
            reactionWheelAngularVelocity2, reactionWheelAngularAcceleration2);

    effectiveMagneticTorque2 = actuatorTorque2.col(0);
    reactionWheelTorque2 = actuatorTorque2.col(1);

    REQUIRE(effectiveMagneticTorque1(0) == Approx(2e-06).epsilon(1e-4));
    REQUIRE(effectiveMagneticTorque1(1) == Approx(-1e-06).epsilon(1e-4));
    REQUIRE(effectiveMagneticTorque1(2) == Approx(0).epsilon(1e-4));

    REQUIRE(reactionWheelTorque1(0) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque1(1) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque1(2) == Approx(1.1e-05).epsilon(1e-4));

    REQUIRE(effectiveMagneticTorque2(0) == Approx(-1.0e-05 * 0.32906).epsilon(1e-4));
    REQUIRE(effectiveMagneticTorque2(1) == Approx(-1.0e-05 * 0.80912).epsilon(1e-4));
    REQUIRE(effectiveMagneticTorque2(2) == Approx(-1.0e-05 * 0.87793).epsilon(1e-4));

    REQUIRE(reactionWheelTorque2(0) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque2(1) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque2(2) == Approx(1.0e-04 * 0.140018).epsilon(1e-4));
}

TEST_CASE("Magnetorquer Plus RW Strategy - Magnetorquers scaling test") {
    Vector3f M = {0.3, 0.18, -0.21};

    Vector3f scaledDipole = magnetorquerPlusRW.scaleMagnetorquerDipole(M);

    Vector3f M1 = {-0.3, 0.18, 0};
    Vector3f scaledDipole1 = magnetorquerPlusRW.scaleMagnetorquerDipole(M1);

    REQUIRE(scaledDipole(0) == Approx(0.2).epsilon(1e-4));
    REQUIRE(scaledDipole(1) == Approx(0.18).epsilon(1e-4));
    REQUIRE(scaledDipole(2) == Approx(-0.2).epsilon(1e-4));

    REQUIRE(scaledDipole1(0) == Approx(-0.2).epsilon(1e-4));
    REQUIRE(scaledDipole1(1) == Approx(0.18).epsilon(1e-4));
    REQUIRE(scaledDipole1(2) == Approx(0).epsilon(1e-4));
}

TEST_CASE("Magnetorquer Plus RW Strategy - Reaction Wheel scaling test") {
    Vector3f reactionWheelTorque = {0.0, 0.0, -1.5e-04};

    Vector3f reactionWheelTorqueScaled = magnetorquerPlusRW.scaleReactionWheelTorque(reactionWheelTorque);

    Vector3f reactionWheelTorque1 = {0.0, 0.0, 1e-05};
    Vector3f reactionWheelTorqueScaled1 = magnetorquerPlusRW.scaleReactionWheelTorque(reactionWheelTorque1);

    REQUIRE(reactionWheelTorqueScaled(0) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorqueScaled(1) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorqueScaled(2) == Approx(-1e-04).epsilon(1e-4));

    REQUIRE(reactionWheelTorqueScaled1(0) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorqueScaled1(1) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorqueScaled1(2) == Approx(1e-05).epsilon(1e-4));
}

TEST_CASE("Magnetorquer Plus RW Strategy - Actuation") {
    Vector3f commandedTorque = {-0.0599, -0.0579, 0.0418};
    Vector3f magneticField = {1.0e-04 * 0.4485, -1.0e-04 * 0.0267, 1.0e-04 * 0.1426};
    bool firstTime = false;
    float currentReactionWheelAngularVelocity = 0;
    float oldReactionWheelAcceleration = 0;

    auto actuatorTorque = magnetorquerPlusRW.actuate(commandedTorque,
                                                     magneticField, firstTime,
                                                     currentReactionWheelAngularVelocity,
                                                     oldReactionWheelAcceleration);

    Vector3f magnetorquerTorque = actuatorTorque(seq(0, 2), 0);
    Vector3f reactionWheelTorque = actuatorTorque(seq(0, 2), 1);

    Vector3f commandedTorque1 = {1.0e-04 * 0.69835, -1.0e-04 * 0.0085, -1.0e-04 * 0.7428};
    Vector3f magneticField1 = {1.0e-04 * 0.44445, -1.0e-04 * 0.03523, -1.0e-04 * 0.14898};
    bool firstTime1 = false;
    float currentReactionWheelAngularVelocity1 = 2.60445 * 1e+02;
    float oldReactionWheelAcceleration1 = 89.77606;

    auto actuatorTorque1 = magnetorquerPlusRW.actuate(commandedTorque1,
                                                      magneticField1, firstTime1,
                                                      currentReactionWheelAngularVelocity1,
                                                      oldReactionWheelAcceleration1);

    Vector3f magnetorquerTorque1 = actuatorTorque1(seq(0, 2), 0);
    Vector3f reactionWheelTorque1 = actuatorTorque1(seq(0, 2), 1);

    Vector3f commandedTorque2 = {1.0e-04 * 0.69835, -1.0e-04 * 0.0085, -1.0e-04 * 0.7428};
    Vector3f magneticField2 = {1.0e-04 * 0.44445, -1.0e-04 * 0.03523, -1.0e-04 * 0.14898};
    bool firstTime2 = true;
    float currentReactionWheelAngularVelocity2 = 2.60445 * 1e+02;
    float oldReactionWheelAcceleration2 = 89.77606;

    auto actuatorTorque2 = magnetorquerPlusRW.actuate(commandedTorque2,
                                                      magneticField2, firstTime2,
                                                      currentReactionWheelAngularVelocity2,
                                                      oldReactionWheelAcceleration2);

    Vector3f magnetorquerTorque2 = actuatorTorque2(seq(0, 2), 0);
    Vector3f reactionWheelTorque2 = actuatorTorque2(seq(0, 2), 1);

    REQUIRE(magnetorquerTorque1(0) == Approx(1.0e-05 * 0.281412).epsilon(1e-4));
    REQUIRE(magnetorquerTorque1(1) == Approx(-1.0e-05 * 0.29944).epsilon(1e-4));
    REQUIRE(magnetorquerTorque1(2) == Approx(1.0e-05 * 0.91034).epsilon(1e-4));

    REQUIRE(reactionWheelTorque1(0) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque1(1) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque1(2) == Approx(-1e-05 * 0.9093353).epsilon(1e-4));

    REQUIRE(magnetorquerTorque2(0) == Approx(1.0e-05 * 0.281412).epsilon(1e-4));
    REQUIRE(magnetorquerTorque2(1) == Approx(-1.0e-05 * 0.29944).epsilon(1e-4));
    REQUIRE(magnetorquerTorque2(2) == Approx(1.0e-05 * 0.91034).epsilon(1e-4));

    REQUIRE(reactionWheelTorque2(0) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque2(1) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque2(2) == Approx(-1e-05 * 0.9093353).epsilon(1e-4));
}