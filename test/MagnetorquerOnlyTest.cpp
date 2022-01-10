#include <catch2/catch.hpp>
#include "MagnetorquerOnly.hpp"

using namespace Eigen;

const MagnetorquerOnly magnetorquerOnly;

TEST_CASE("Magnetorquer Only Strategy - Magnetorquer saturation") {
    Vector3f commandedTorque1 = {5e-06, 5e-06, 0};
    Vector3f magneticField1 = {0, 0, 1e-05};
    Vector3f desiredMagneticDipole1 = {-0.5, 0.5, 0};
    Vector3f desiredReactionWheelTorque1 = {0, 0, 0};
    Vector3f desiredMagneticTorque1 = {5e-06, 5e-06, 0};

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque1 = magnetorquerOnly.desaturateMagnetorquer(
            desiredMagneticTorque1,
            desiredReactionWheelTorque1,
            commandedTorque1,
            magneticField1,
            desiredMagneticDipole1);
    desiredMagneticTorque1 = actuatorTorque1.col(0);

    Vector3f commandedTorque2 = {-1e-03 * 0.40561, -1e-03 * 0.55451, 1e-03 * 0.29295};
    Vector3f magneticField2 = {1.0e-04 * 0.44848, -1.0e-04 * 0.02672, -1.0e-04 * 0.14255};
    Vector3f desiredMagneticDipole2 = {-2.148192, 26.263678, -11.681327};
    Vector3f desiredReactionWheelTorque2 = {0, 0, 0};
    Vector3f desiredMagneticTorque2 = {-0.000405, -0.000554, -0.001172};

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque2 = magnetorquerOnly.desaturateMagnetorquer(
            desiredMagneticTorque2,
            desiredReactionWheelTorque2,
            commandedTorque2,
            magneticField2,
            desiredMagneticDipole2);
    desiredMagneticTorque2 = actuatorTorque2.col(0);

    REQUIRE(desiredMagneticTorque1(0) == Approx(1.5e-06).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque1(1) == Approx(1.5e-06).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque1(2) == Approx(0).epsilon(1e-4));

    REQUIRE(desiredMagneticTorque2(0) == Approx(-1.0e-04 * 0.038610).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque2(1) == Approx(-1.0e-04 * 0.052783).epsilon(1e-4));
    REQUIRE(desiredMagneticTorque2(2) == Approx(-1.0e-04 * 0.111574).epsilon(1e-4));

    SECTION("Zero desired dipole in all axes") {
        Vector3f commandedTorque = {5e-06, 5e-06, 0};
        Vector3f magneticField = {0, 0, 1e-05};
        Vector3f desiredMagneticDipole = {0, 0, 0};
        Vector3f desiredReactionWheelTorque = {0, 0, 0};
        Vector3f desiredMagneticTorque = {-0.000405, -0.000554, -0.001172};

        Matrix<float, VectorSize, NumOfActuators> actuatorTorque = magnetorquerOnly.desaturateMagnetorquer(
                desiredMagneticTorque,
                desiredReactionWheelTorque,
                commandedTorque,
                magneticField,
                desiredMagneticDipole);
        desiredMagneticTorque = actuatorTorque.col(0);

        REQUIRE(desiredMagneticTorque(0) == Approx(-0.000405).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(1) == Approx(-0.000554).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(2) == Approx(-0.001172).epsilon(1e-4));
    }

    SECTION("Zero desired dipole in two axes - Division by zero") {
        Vector3f commandedTorque = {5e-06, 5e-06, 0};
        Vector3f magneticField = {0, 0, 1e-05};
        Vector3f desiredMagneticDipole = {0, 0, 0.5};
        Vector3f desiredReactionWheelTorque = {0, 0, 0};
        Vector3f desiredMagneticTorque = {-0.000405, -0.000554, -0.001172};

        Matrix<float, VectorSize, NumOfActuators> actuatorTorque = magnetorquerOnly.desaturateMagnetorquer(
                desiredMagneticTorque,
                desiredReactionWheelTorque,
                commandedTorque,
                magneticField,
                desiredMagneticDipole);
        desiredMagneticTorque = actuatorTorque.col(0);

        REQUIRE(desiredMagneticTorque(0) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(1) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(2) == Approx(0).epsilon(1e-4));
    }

    SECTION("Zero desired dipole in one axis - Division by zero") {
        Vector3f commandedTorque = {5e-06, 5e-06, 0};
        Vector3f magneticField = {0, 0, 1e-05};
        Vector3f desiredMagneticDipole = {0, 0.5, 0.5};
        Vector3f desiredReactionWheelTorque = {0, 0, 0};
        Vector3f desiredMagneticTorque = {-0.000405, -0.000554, -0.001172};

        Matrix<float, VectorSize, NumOfActuators> actuatorTorque = magnetorquerOnly.desaturateMagnetorquer(
                desiredMagneticTorque,
                desiredReactionWheelTorque,
                commandedTorque,
                magneticField,
                desiredMagneticDipole);
        desiredMagneticTorque = actuatorTorque.col(0);

        REQUIRE(desiredMagneticTorque(0) == Approx(2.5e-06).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(1) == Approx(0).epsilon(1e-4));
        REQUIRE(desiredMagneticTorque(2) == Approx(0).epsilon(1e-4));
    }
}

TEST_CASE("Magnetorquer Only Strategy - Actuation") {
    Vector3f commandedTorque = {-0.0599, -0.0579, 0.0418};
    Vector3f magneticField = {1.0e-04 * 0.4485, -1.0e-04 * 0.0267, -1.0e-04 * 0.1426};
    float currentReactionWheelAngularVelocity = 0;
    float oldReactionWheelAcceleration = 0;
    bool firstTime = false;

    auto actuatorTorque = magnetorquerOnly.actuate(commandedTorque,
                                                   magneticField, firstTime,
                                                   currentReactionWheelAngularVelocity,
                                                   oldReactionWheelAcceleration);

    Vector3f magnetorquerTorque = actuatorTorque(seq(0, 2), 0);


    Vector3f commandedTorque1 = {1.0e-04 * 0.69835, -1.0e-04 * 0.0085, -1.0e-04 * 0.7428};
    Vector3f magneticField1 = {1.0e-04 * 0.44445, -1.0e-04 * 0.03523, -1.0e-04 * 0.14898};

    auto actuatorTorque1 = magnetorquerOnly.actuate(commandedTorque1,
                                                    magneticField1, firstTime,
                                                    currentReactionWheelAngularVelocity,
                                                    oldReactionWheelAcceleration);

    Vector3f magnetorquerTorque1 = actuatorTorque1(seq(0, 2), 0);

    REQUIRE(magnetorquerTorque(0) == Approx(1.0e-04 * 0.00970864).epsilon(1e-4));
    REQUIRE(magnetorquerTorque(1) == Approx(-1.0e-04 * 0.104102).epsilon(1e-4));
    REQUIRE(magnetorquerTorque(2) == Approx(1.0e-04 * 0.050027).epsilon(1e-4));

    REQUIRE(magnetorquerTorque1(0) == Approx(-1.0e-05 * 0.3074627).epsilon(1e-4));
    REQUIRE(magnetorquerTorque1(1) == Approx(-1.0e-05 * 0.1533511).epsilon(1e-4));
    REQUIRE(magnetorquerTorque1(2) == Approx(-1.0e-05 * 0.8809856).epsilon(1e-4));
}