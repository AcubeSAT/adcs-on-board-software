#include "CSSCompensation.hpp"
#include "Eigen/Geometry"
#include "MathFunctions.hpp"

using namespace Eigen;

Vector3f
cssCompensation(Vector3f sunPositionBody, Quaternionf quaternionECIBody, Vector3f satellitePositionECI, float albedo) {
    sunPositionBody.normalize();
    satellitePositionECI.normalize();

    Vector3f satellitePositionBody = rotateVector(quaternionECIBody, satellitePositionECI);
    Vector3f nadir = -satellitePositionBody;

    std::array<Quaternionf, 6> cssFrameQuatRot;

    Quaternionf quaternion;
    quaternion.w() = cos(-PI / 4);
    quaternion.vec() = sin(-PI / 4) * Vector3f(0, 1, 0);
    cssFrameQuatRot[0] = quaternion;

    quaternion.w() = cos(PI / 4);
    quaternion.vec() = sin(PI / 4) * Vector3f(0, 1, 0);
    cssFrameQuatRot[1] = quaternion;

    quaternion.w() = cos(-PI / 2);
    quaternion.vec() = sin(-PI / 2) * Vector3f(0, 1, 0);
    cssFrameQuatRot[2] = quaternion;

    quaternion.w() = cos(-PI / 4);
    quaternion.vec() = sin(-PI / 4) * Vector3f(0, 0, 1);
    cssFrameQuatRot[3] = quaternion;

    quaternion.w() = cos(PI / 4);
    quaternion.vec() = sin(PI / 4) * Vector3f(0, 0, 1);
    cssFrameQuatRot[4] = quaternion;

    Matrix<float, 3, 6> sunPositionInFrame;
    Matrix<float, 3, 6> nadirInFrame;

    sunPositionInFrame(seq(0, 2), 0) = sunPositionBody;
    nadirInFrame(seq(0, 2), 0) = nadir;

    for (int i = 0; i < 5; i++) {
        sunPositionInFrame(seq(0, 2), i + 1) = rotateVector(cssFrameQuatRot[i], sunPositionBody);
        nadirInFrame(seq(0, 2), i + 1) = rotateVector(cssFrameQuatRot[i], nadir);
    }

    Vector<float, 6> currentSun;
    Vector<float, 6> currentAlbedo;
    Vector<float, 6> totalCurrent;

    for (int i = 0; i < 6; i++) {
        currentSun(i) = sunPositionInFrame(seq(0, 2), i).dot(Vector3f(1, 0, 0));

        currentSun(i) = clampDown(currentSun(i), 0.0f);

        currentAlbedo(i) = albedo * nadirInFrame(seq(0, 2), i).dot(Vector3f(1, 0, 0));

        currentAlbedo(i) = clampDown(currentAlbedo(i), 0.0f);

        totalCurrent(i) = currentSun(i) + currentAlbedo(i);
    }

    Vector3f totalSunVector(
            totalCurrent(0) - totalCurrent(3),
            totalCurrent(5) - totalCurrent(4),
            totalCurrent(1) - totalCurrent(2)
    );
    totalSunVector.normalize();

    return totalSunVector;
}


