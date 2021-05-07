#include <iostream>
#include "mathFunctions.h"

#define STATE_SIZE 6
#define dt 0.1
#define Q_size 2

#include "lib/eigen/Eigen/Dense"
#include "inc/SatelliteModel.h"

using namespace Eigen;

void test_function(float *argument, float *argument2) {
    std::cout << "success";
}


int main() {

    float timestep = 0.1;
    SatelliteModel model = SatelliteModel(timestep);
//    state = satellite_model.state_transition_function(state, gyro_measurements);
//    //std::cout << state;
//
//    Matrix<float, LOCAL_STATE_SIZE, LOCAL_STATE_SIZE> jacobian = satellite_model.state_transition_jacobian(
//            state,
//            gyro_measurements);
//
//    //std::cout << jacobian;
//
    const Vector<float, 3> mag_ref(-0.7701, -0.2939,-0.5662);
    const Vector<float, 3> sun_ref(-0.1722, 0.9038,0.3918);
    const Vector<float, 3> gyro(0.0024, 0.1432,-0.0972);

    int eclipse = 0;

    Vector<float, 7> state(0.4524, -0.1166, 0.8841, -0.0084, 0, 0, 0);
    const Vector<float, 3> gyro_measurements(0.0024, 0.1432,-0.0972);
    Quaternionf quat(0.4095, -0.0430, 0.9095, 0.0571);

    state(0) = quat.w();
    state(seq(1,3)) = quat.vec();

    Matrix<float, 6, 6> measurement_jacobian = model.measurementJacobian(mag_ref,sun_ref,eclipse,state);
    std::cout << measurement_jacobian;

    return 0;
}