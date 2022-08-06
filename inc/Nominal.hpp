#pragma once

#include "EnvironmentalModel.hpp"
#include "MEKF.hpp"
#include "Parameters.hpp"


 /**
  * Main function for Nominal Mode
  * @param numberOfCycles The number of Cycles, that we need to test the Nominal mode
  * @return Mekf's global state vector
  */
GlobalStateVector NominalMode(int numberOfCycles);