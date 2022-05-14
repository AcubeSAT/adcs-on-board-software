#include <catch2/catch.hpp>
#include "Detumbling.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

TEST_CASE("Torque Test") {

    Vector3f magneticFieldBody1 = {-1.44505468032163e-5, 1.77939963990674e-5, -3.95126047327029e-6};

    Vector3f magneticFieldBody2 = {-1.37815279474949e-5, 1.83322872033822e-5, -4.11412598905325e-6};

    Bdot bDot(magneticFieldBody1, magneticFieldBody2);

    Vector3f magneticTorque = calculateDesiredMagneticTorque(bDot);

    REQUIRE(magneticTorque[0] * 10000000 == Approx(-4.55641919081183).epsilon(1e-3));
    REQUIRE(magneticTorque[1] * 1000000 == Approx(-1.80204558688336).epsilon(1e-3));
    REQUIRE(magneticTorque[2] * 1000000 == Approx(-6.44890864045674).epsilon(1e-3));
}

TEST_CASE("Angular Velocity Test") {
    Vector3f magneticFieldBody1 = {1.52403644509235e-5, 1.76439571689881e-5, -1.13221000566922e-6};

    Vector3f magneticFieldBody2 = {1.60088168745433e-5, 1.69615650217286e-5, -8.91850656074703e-7};

    Bdot bDot(magneticFieldBody1, magneticFieldBody2);

    Vector3f magneticTorque = calculateDesiredMagneticTorque(bDot);
    Vector3f angularVelocity = estimateAngularVelocity(bDot.getBDotVector(), bDot.getSamplingEndMagneticFieldBody());

    REQUIRE(angularVelocity[0] == Approx(-0.0636545152441620).epsilon(1e-3));
    REQUIRE(angularVelocity[1] == Approx(0.0831996293001571).epsilon(1e-3));
    REQUIRE(angularVelocity[2] == Approx(0.439717616066728).epsilon(1e-3));
}

TEST_CASE("Detumbling Case 1") {
    Bdot bdot({0, 0, 0}, {0, 0, 0});
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData = Matrix<float, 180, 288>::Identity() * 100000;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData);

    Quaternionf q_body_eci({0.3757, 0.5983, -0.2364, -0.6671});
    Vector3f omega = {PI / 30, PI / 30, PI / 30};
    Vector3f omega_dot = {0, 0, 0};
    Matrix<float, 3, 3> Inertia({
                                        {0.0412158223637031,    -1.85801942968305e-06, 0.00137245665256380},
                                        {-1.85801942968300e-06, 0.0410190161501943,    -1.64802792093441e-05},
                                        {0.00137245665256380,   -1.64802792093441e-05, 0.00695520282610255}
                                });
    Matrix<float, 3, 3> I = Matrix<float, 3, 3>::Identity();
    double dt = 0.1;
    bool switchToNominal = false;
    Vector3f magneticFieldBody1 = {0, 0, 0};
    Vector3f magneticFieldBody2 = {0, 0, 0};
    Vector3f desiredTorque = {0, 0, 0};
    Vector3f w_estim = {100, 100, 100};
    Quaternionf q_dot = {0, omega.x(), omega.y(), omega.z()};
    float trigger = 0.035;

    while (!switchToNominal) {
        em.ModelEnvironment();
        magneticFieldBody1 = rotateVector(q_body_eci, em.getMagneticField());
        bdot.setSamplingBeginMagneticFieldBody(magneticFieldBody1);
        q_dot = quaternionProduct(q_body_eci, q_dot);
        q_dot.coeffs() = q_dot.coeffs() * 0.5;
        q_body_eci.coeffs() += q_dot.coeffs() * dt;
        q_body_eci.normalize();
        em.ModelEnvironment();
        magneticFieldBody2 = rotateVector(q_body_eci, em.getMagneticField());
        bdot.setSamplingEndMagneticFieldBody(magneticFieldBody2);
        desiredTorque = calculateDesiredMagneticTorque(bdot);
        w_estim = estimateAngularVelocity(bdot.getBDotVector(), bdot.getSamplingBeginMagneticFieldBody());

        // ACTUATE

        for (int i = 0; i < 7; ++i) {
            em.ModelEnvironment();
        }

        omega = 7 * 0.5 * omega * dt +
                0.5 * Inertia.inverse() * (desiredTorque - omega.cross(Inertia * omega)) * (0.7 * dt) * (0.7 * dt);
        q_dot = {0, omega.x(), omega.y(), omega.z()};
        q_dot.normalize();
        q_body_eci = quaternionProduct(q_body_eci, q_dot);
        q_body_eci.normalize();
        switchToNominal = ((abs(w_estim.x()) < trigger) && (abs(w_estim.y()) < trigger) &&
                            (abs(w_estim.z()) < trigger));
    }

    REQUIRE(switchToNominal);
}


TEST_CASE("Detumbling Case 2") {
    Bdot bdot({0, 0, 0}, {0, 0, 0});
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData = Matrix<float, 180, 288>::Identity() * 100000;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData);

    Quaternionf q_body_eci({0.71772,0.449563,0.177631,0.501259});
    Vector3f omega = {PI / 3, PI / 3, PI / 3};
    Vector3f omega_dot = {0, 0, 0};
    Matrix<float, 3, 3> Inertia({
                                        {0.0412158223637031,    -1.85801942968305e-06, 0.00137245665256380},
                                        {-1.85801942968300e-06, 0.0410190161501943,    -1.64802792093441e-05},
                                        {0.00137245665256380,   -1.64802792093441e-05, 0.00695520282610255}
                                });
    Matrix<float, 3, 3> I = Matrix<float, 3, 3>::Identity();
    double dt = 0.1;
    bool switchToNominal = false;
    Vector3f magneticFieldBody1 = {0, 0, 0};
    Vector3f magneticFieldBody2 = {0, 0, 0};
    Vector3f desiredTorque = {0, 0, 0};
    Vector3f w_estim = {100, 100, 100};
    Quaternionf q_dot = {0, omega.x(), omega.y(), omega.z()};
    float trigger = 0.035;

    while (!switchToNominal) {
        em.ModelEnvironment();
        magneticFieldBody1 = rotateVector(q_body_eci, em.getMagneticField());
        bdot.setSamplingBeginMagneticFieldBody(magneticFieldBody1);
        q_dot = quaternionProduct(q_body_eci, q_dot);
        q_dot.coeffs() = q_dot.coeffs() * 0.5;
        q_body_eci.coeffs() += q_dot.coeffs() * dt;
        q_body_eci.normalize();
        em.ModelEnvironment();
        magneticFieldBody2 = rotateVector(q_body_eci, em.getMagneticField());
        bdot.setSamplingEndMagneticFieldBody(magneticFieldBody2);
        desiredTorque = calculateDesiredMagneticTorque(bdot);
        w_estim = estimateAngularVelocity(bdot.getBDotVector(), bdot.getSamplingBeginMagneticFieldBody());


        // ACTUATE

        for (int i = 0; i < 7; ++i) {
            em.ModelEnvironment();
        }

        omega = 7 * 0.5 * omega * dt +
                0.5 * Inertia.inverse() * (desiredTorque - omega.cross(Inertia * omega)) * (0.7 * dt) * (0.7 * dt);
        q_dot = {0, omega.x(), omega.y(), omega.z()};
        q_dot.normalize();
        q_body_eci = quaternionProduct(q_body_eci, q_dot);
        q_body_eci.normalize();
        switchToNominal = ((abs(w_estim.x()) < trigger) && (abs(w_estim.y()) < trigger) &&
                           (abs(w_estim.z()) < trigger));
    }

    REQUIRE(switchToNominal);
}