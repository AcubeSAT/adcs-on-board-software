#pragma once

#include "EnvironmentalModel.hpp"
#include "MEKF.hpp"
#include "Parameters.hpp"


/**
 * Main function for Nominal Mode
 */
GlobalStateVector NominalMode(int numberOfCycles);