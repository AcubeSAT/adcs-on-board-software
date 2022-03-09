#include "Geomag.hpp"

// This C99 file uses designated initializers to initialize the IGRF state.
// This was done to mimick the original Geomag.c file behavior, where
// the state vector was initialized in the exact same way.

// See https://web.archive.org/web/20211008081131/https://stackoverflow.com/questions/855996/c-equivalent-to-designated-initializers
// to understand how this file is used in Geomag.cpp/Geomag.hpp.

geomag_vector igrf_vector = {.currentDate = 0, .latitude = 0, .longitude = 0,
	.altitude = 0, .xMagneticField = 0,
	.yMagneticField = 0, .zMagneticField = 0,
	.norm = 0, .declination = 0, .inclination = 0,
	.horizontalIntensity = 0, .totalIntensity = 0};