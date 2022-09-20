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


Matrix<float, 5, 3> actualVelocitiesCase1({
                                                  {0.269359418811723,  -0.700790251161457, 0.503457016937098},
                                                  {-0.029937827090688, -0.758895982402411, 0.487505906399663},
                                                  {-0.327542432465865, -0.692803003941368, 0.479592726673585},
                                                  {-0.577904225133820, -0.510357998047160, 0.478777086553297},
                                                  {-0.736488722611760, -0.236644565280844, 0.480489855856063}
                                          });

Matrix<float, 5, 3> actualVelocitiesCase2({
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

    Vector3f angularVelocity = estimateAngularVelocity(bDot.getBDotVector(),
                                                       bDot.getSamplingEndMagneticFieldBody());

    REQUIRE(angularVelocity[0] == Approx(-0.0636545152441620).epsilon(1e-3));
    REQUIRE(angularVelocity[1] == Approx(0.0831996293001571).epsilon(1e-3));
    REQUIRE(angularVelocity[2] == Approx(0.439717616066728).epsilon(1e-3));
}
