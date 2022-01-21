#pragma once 

#include "Eigen/Geometry"


/**
 * Calculates the theoretical reflected sunlight as a percentage of the direct sunlight, and then removes it from the coarse sun sensor measurements before creating the final sun vector.
 * @param sunPositionBody sun position in Body frame
 * @param quaternionECIBody quaternion that performs the rotation from ECI to Body frame
 * @param satellitePositionECI satellite position in ECI frame
 * @param albedo the fraction of the sunlight reflected off the Earth's surface
 * @return the light the coarse sun sensors detect.
 */
Eigen::Vector3f
cssCompensation(Eigen::Vector3f sunPositionBody, const Eigen::Quaternionf quaternionECIBody, Eigen::Vector3f satellitePositionECI,
                const float albedo);
