#include "GyroBiasFunction.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

Vector3f calculateGyroBias(Quaternionf wahbaOutputQuaternion1,Quaternionf wahbaOutputQuaternion2,Vector3f gyroscopeMeasurement){
    Vector3f gyroscopeBias;
    Quaternionf quaternionDifference,temporaryQuaternion;
    float angularEstimatedRateMean;
    Vector3f angularEstimatedRate;
    quaternionDifference.vec() = wahbaOutputQuaternion2.vec() - wahbaOutputQuaternion1.vec();
    temporaryQuaternion = quaternionProduct(wahbaOutputQuaternion2.conjugate(), quaternionDifference);
    angularEstimatedRate = 2 * temporaryQuaternion.vec();
    angularEstimatedRateMean = angularEstimatedRate.mean();
    for (int i = 0; i < 3; i++) {
        gyroscopeBias[i] = gyroscopeMeasurement[i] - angularEstimatedRateMean;
    }
    return gyroscopeBias;
}
