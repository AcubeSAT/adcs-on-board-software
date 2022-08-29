#include <catch2/catch.hpp>
#include "Detumbling.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

Matrix<float, 3, 3> Inertia({
                                    {0.0412158223637031,    -1.85801942968305e-06, 0.00137245665256380},
                                    {-1.85801942968300e-06, 0.0410190161501943,    -1.64802792093441e-05},
                                    {0.00137245665256380,   -1.64802792093441e-05, 0.00695520282610255}
                            });

uint8_t totalSimulationSteps = 25;
Matrix<float, 3, 3> I = Matrix<float, 3, 3>::Identity();

Matrix<float, 6, 3> actualVelocitiesCase1({
                                                  {0.105511225520354, 0.103969576196999, 0.104521131640659},
                                                  {0.113671139332526, 0.094284264294752, 0.105480114264322},
                                                  {0.120996785659813, 0.083916834778299, 0.106359399415552},
                                                  {0.127445303954953, 0.072960332486723, 0.107244195152626},
                                                  {0.132957923239574, 0.061468592122719, 0.107955187872409},
                                                  {0.137493406985540, 0.049547921018137, 0.108584898816924}
                                          });

Matrix<float, 6, 3> actualVelocitiesCase2({
                                                  {1.046872272855053,  1.046263442203266,  1.046697271787978},
                                                  {1.456298474300431,  -0.131848057089224, 1.088453973477571},
                                                  {0.855610364696138,  -1.216455920963766, 1.033202919604997},
                                                  {-0.309797481211099, -1.490430571170476, 0.968336546045047},
                                                  {-1.314021938877599, -0.807232359642279, 0.961190384951700},
                                                  {-1.482985471324063, 0.434734747733695,  0.965709743415944},
                                          });


TEST_CASE("Torque Test") {
    Vector3f magneticFieldBody1 = {-1.44505468032163e-5, 1.77939963990674e-5, -3.95126047327029e-6};
    Vector3f magneticFieldBody2 = {-1.37815279474949e-5, 1.83322872033822e-5, -4.11412598905325e-6};

    Bdot bDot(magneticFieldBody1, magneticFieldBody2);

    Vector3f magneticTorque = calculateDesiredMagneticTorque(bDot);

    REQUIRE(magneticTorque[0] == Approx(-4.55641919081183e-7).epsilon(1e-3));
    REQUIRE(magneticTorque[1] == Approx(-1.80204558688336e-6).epsilon(1e-3));
    REQUIRE(magneticTorque[2] == Approx(-6.44890864045674e-6).epsilon(1e-3));
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

TEST_CASE("Detumbling Case - Low initial angular velocity") {
    Bdot bdot({0, 0, 0}, {0, 0, 0});
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData = Matrix<float, 180, 288>::Identity() * 100000;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData);

    Quaternionf q_body_eci({0.3757, 0.5983, -0.2364, -0.6671});
    q_body_eci.normalize();
    Vector3f omega = {PI / 30, PI / 30, PI / 30};
    Vector3f omega_dot = {0, 0, 0};


    Vector3f magneticFieldBody1 = {0, 0, 0};
    Vector3f magneticFieldBody2 = {0, 0, 0};
    float timeStep = 0.1;
    for (uint8_t step; step < totalSimulationSteps; step++) {

        Quaternionf omega_quaternion = {0, omega.x(), omega.y(), omega.z()};
        em.ModelEnvironment();
        magneticFieldBody1 = rotateVector(q_body_eci, em.getMagneticField() * 1e-9);
        bdot.setSamplingBeginMagneticFieldBody(magneticFieldBody1);
        if (omega_quaternion.norm() > 1e-9) {
            Quaternionf q_dot = quaternionProduct(q_body_eci, omega_quaternion);
            q_body_eci.coeffs() = (0.5 + timeStep) * q_dot.coeffs();
            q_body_eci.normalize();
        }
        em.ModelEnvironment();
        magneticFieldBody2 = rotateVector(q_body_eci, em.getMagneticField() * 1e-9);
        bdot.setSamplingBeginMagneticFieldBody(magneticFieldBody2);

        Vector3f desiredTorque = calculateDesiredMagneticTorque(bdot);

        Vector3f estimatedAngularVelocity = estimateAngularVelocity(bdot.getBDotVector(),
                                                                    bdot.getSamplingEndMagneticFieldBody());
        float tinyStep = 1e-3;
        for (uint16_t dt = 0; dt < 700; dt++) {
            Vector3f de_omega = Inertia.inverse() * (desiredTorque - omega.cross(Inertia * omega));
            omega += de_omega * tinyStep;
            q_body_eci = quaternionProduct(q_body_eci, quaternionExponent(omega));
            q_body_eci.normalize();
        }

        if (step < 5){
            CHECK(omega.x() == Approx(actualVelocitiesCase1.coeff(step,0)).epsilon(1e-3));
            CHECK(omega.y() == Approx(actualVelocitiesCase1.coeff(step,1)).epsilon(1e-3));
            CHECK(omega.z() == Approx(actualVelocitiesCase1.coeff(step,2)).epsilon(1e-3));
        }

        for(uint8_t i = 0; i < 7; i++){
            em.ModelEnvironment();
        }
    }
}


TEST_CASE("Detumbling Case - Mission Loss Danger") {
    Bdot bdot({0, 0, 0}, {0, 0, 0});
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData = Matrix<float, 180, 288>::Identity() * 100000;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData);

    Quaternionf q_body_eci({0.71772, 0.449563, 0.177631, 0.501259});
    q_body_eci.normalize();
    Vector3f omega = {PI / 3, PI / 3, PI / 3};
    Vector3f omega_dot = {0, 0, 0};

}