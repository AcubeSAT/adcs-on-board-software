#include "wahba.h"
#include "Definitions.h"

Eigen::Quaternionf wahba(Eigen::Vector3f magneticBody, Eigen::Vector3f magneticECI, Eigen::Vector3f sunBody,
                         Eigen::Vector3f sunECI) {
    magneticBody.normalize();
    magneticECI.normalize();
    sunBody.normalize();
    sunECI.normalize();

    Eigen::Matrix<float, VectorSize, VectorSize> A;

    for (uint8_t i = 0; i < VectorSize; i++) {
        A(i, 0) = magneticBody(i) * magneticECI(0) +
                  sunBody(i) * sunECI(0);
        A(i, 1) = magneticBody(i) * magneticECI(1) +
                  sunBody(i) * sunECI(1);
        A(i, 2) = magneticBody(i) * magneticECI(2) +
                  sunBody(i) * sunECI(2);
    }

    Eigen::JacobiSVD<Eigen::MatrixXf> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::Matrix<float, VectorSize, VectorSize> U = svd.matrixU();
    Eigen::Matrix<float, VectorSize, VectorSize> V = svd.matrixV();

    float d = U.determinant() * V.determinant();
    if (d < 0) {
        U(0, 2) *= -1;
        U(1, 2) *= -1;
        U(2, 2) *= -1;
    }

    Eigen::Matrix<float, VectorSize, VectorSize> rotationMatrix = U * V.transpose();

    Eigen::Quaternionf finalQuaternion(rotationMatrix.transpose());

    return finalQuaternion;
}