#pragma once

typedef struct geomag_vector {
    double currentDate; /**< Date (decimal) */
    double latitude; /**< Geodetic latitude (degrees) */
    double longitude; /**< Geodetic longitude (degrees) */
    double altitude; /**< Altitude over earth's surface (km) */
    double xMagneticField; /**< Northward component (nT) */
    double yMagneticField; /**< Eastward component (nT) */
    double zMagneticField; /**< Vertical component (nT) */
    double norm; /**< Magnitude of X,Y,Z magnetic field*/
    double declination; /**< declination (degrees) */
    double inclination; /**< inclination (degrees) */
    double horizontalIntensity; /**< horizontalIntensity (nT) */
    double totalIntensity; /**< totalIntensity (nT) */
} geomag_vector;

extern geomag_vector igrf_vector;

void geomag(geomag_vector *gStr);
