#include "Geomag.h"
#include "Definitions.hpp"
#include "MathFunctions.hpp"

#define GEO_NAN log(-1.0)

geomag_vector igrf_vector = {.currentDate = 0, .latitude = 0, .longitude = 0,
        .altitude = 0, .xMagneticField = 0,
        .yMagneticField = 0, .zMagneticField = 0,
        .norm = 0, .declination = 0, .inclination = 0,
        .horizontalIntensity = 0, .totalIntensity = 0};

static int interpsh(double date, double dte1, int nmax1, double dte2, int nmax2,
                    int gh);

static int extrapsh(double date, double dte1, int nmax1, int nmax2, int gh);

static int shval3(uint8_t igdgc, double flat, double flon, double elev, int nmax,
                  int gh, int iext, double ext1, double ext2, double ext3);

static int dihf(int gh);

static int getshc(int iflag, int nmax_of_gh, int gh);

static const float COEFF1[] = {-29404.80, -1450.90, -2499.60, 2982.00, 1677.00,
                               1363.20, -2381.20, 1236.20, 525.70, 903.00, 809.50, 86.30, -309.40,
                               48.0, -234.30, 363.20, 187.80, -140.70, -151.20, 13.50, 66.00, 65.50,
                               72.90, -121.50, -36.20, 13.50, -64.70, 80.60, -76.70, -8.20, 56.50,
                               15.80, 6.40, -7.20, 9.80, 23.70, 9.70, -17.60, -0.50, -21.10, 15.30,
                               13.70, -16.50, -0.30, 5.00, 8.40, 2.90, -1.50, -1.10, -13.20, 1.10,
                               8.80, -9.30, -11.90, -1.90, -6.20, -0.10, 1.70, -0.90, 0.70, -0.90, 1.90,
                               1.40, -2.40, -3.80, 3.00, -1.40, -2.50, 2.30, -0.90, 0.30, -0.70, -0.10,
                               1.40, -0.60, 0.20, 3.10, -2.00, -0.10, 0.50, 1.30, -1.20, 0.70, 0.30,
                               0.50, -0.30, -0.50, 0.10, -1.10, -0.30, 0.10, -0.90, 0.50, 0.70, -0.30,
                               0.80, 0.00, 0.80, 0.00, 0.40, 0.10, 0.50, -0.50, -0.40};

static const float COEFF2[] = {0.00, 4652.50, 0.00, -2991.60, -734.60, 0.00, -82.10,
                               241.90, -543.40, 0.00, 281.90, -158.40, 199.70, -349.70, 0.00, 47.70,
                               208.30, -121.20, 32.30, 98.90, 0.00, -19.10, 25.10, 52.80, -64.50,
                               8.90, 68.10, 0.00, -51.50, -16.90, 2.20, 23.50, -2.20, -27.20, -1.80,
                               0.00, 8.40, -15.30, 12.80, -11.70, 14.90, 3.60, -6.90, 2.80, 0.00,
                               -23.40, 11.00, 9.80, -5.10, -6.30, 7.80, 0.40, -1.40, 9.60, 0.00, 3.40,
                               -0.20, 3.60, 4.80, -8.60, -0.10, -4.30, -3.40, -0.10, -8.80, 0.00,
                               0.00, 2.50, -0.60, -0.40, 0.60, -0.20, -1.70, -1.60, -3.00, -2.00,
                               -2.60, 0.00, -1.20, 0.50, 1.40, -1.80, 0.10, 0.80, -0.20, 0.60, 0.20,
                               -0.90, 0.00, 0.50, 0.00, -0.90, 0.60, 1.40, -0.40, -1.30, -0.10, 0.30,
                               -0.10, 0.50, 0.50, -0.40, -0.40, -0.60};

static const float COEFF3[] = {5.70, 7.40, -11.00, -7.00, -2.10, 2.20, -5.90, 3.10,
                               -12.00, -1.20, -1.60, -5.90, 5.20, -5.10, -0.30, 0.50, -0.60, 0.20,
                               1.30, 0.90, -0.50, -0.30, 0.40, 1.30, -1.40, 0.00, 0.90, -0.10, -0.20,
                               0.00, 0.70, 0.10, -0.50, -0.80, 0.80, 0.00, 0.10, -0.10, 0.40, -0.10,
                               0.40, 0.30, -0.10, 0.40, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                               0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                               0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                               0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                               0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                               0.00, 0.00, 0.00, 0.00};

static const float COEFF4[] = {0.00, -25.90, 0.00, -30.20, -22.40, 0.00, 6.00, -1.10,
                               0.50, 0.00, -0.10, 6.50, 3.60, -5.00, 0.00, 0.00, 2.50, -0.60, 3.00,
                               0.30, 0.00, 0.00, -1.60, -0.30, 0.80, 0.00, 1.00, 0.00, 0.60, 0.60,
                               -0.80, -0.20, -1.10, 0.10, 0.30, 0.00, -0.20, 0.60, -0.20, 0.50, -0.30,
                               -0.40, 0.50, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                               0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                               0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                               0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                               0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
                               0.00, 0.00, 0.00};

#define IEXT    0

#define EXT_COEFF1 (double)0
#define EXT_COEFF2 (double)0
#define EXT_COEFF3 (double)0

#define MAXDEG 13
#define MAXCOEFF (MAXDEG*(MAXDEG+2)+1) /* index starts with 1!, (from old Fortran?) */
static double gh1[MAXCOEFF];
static double gh2[MAXCOEFF];
static double gha[MAXCOEFF]; /* Geomag global variables */
static double ghb[MAXCOEFF];
static double d = 0, f = 0, h = 0, i = 0;
static double dtemp, ftemp, htemp, itemp;
static double x = 0, y = 0, z = 0;
static double xtemp, ytemp, ztemp;
static double PREV_SDATE = 0;

/****************************************************************************/
/*                                                                          */
/*                             Program Geomag                               */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*      This program, originally written in FORTRAN, was developed using    */
/*      subroutines written by                                              */
/*      A. Zunde                                                            */
/*      USGS, MS 964, Box 25046 Federal Center, Denver, Co.  80225          */
/*      and                                                                 */
/*      S.R.C. Malin & D.R. Barraclough                                     */
/*      Institute of Geological Sciences, United Kingdom.                   */
/*                                                                          */
/*      Translated                                                          */
/*      into C by    : Craig H. Shaffer                                     */
/*                     29 July, 1988                                        */
/*                                                                          */
/*      Rewritten by : David Owens                                          */
/*                     For Susan McLean                                     */
/*                                                                          */
/*      Maintained by: Stefan Maus                                          */
/*      Contact      : stefan.maus@noaa.gov                                 */
/*                     National Geophysical Data Center                     */
/*                     World Data Center-A for Solid Earth Geophysics       */
/*                     NOAA, E/GC1, 325 Broadway,                           */
/*                     Boulder, CO  80303                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*      Some variables used in this program                                 */
/*                                                                          */
/*    Name         Type                    Usage                            */
/* ------------------------------------------------------------------------ */
/*                                                                          */
/*   a2,b2      Scalar Double          Squares of semi-major and semi-minor */
/*                                     axes of the reference spheroid used  */
/*                                     for transforming between geodetic or */
/*                                     geocentric coordinates.              */
/*                                                                          */
/*   minalt     Double array of MAXMOD Minimum height of model.             */
/*                                                                          */
/*   altmin     Double                 Minimum height of selected model.    */
/*                                                                          */
/*   altmax     Double array of MAXMOD Maximum height of model.             */
/*                                                                          */
/*   maxalt     Double                 Maximum height of selected model.    */
/*                                                                          */
/*   d          Scalar Double          Declination of the field from the    */
/*                                     geographic north (deg).              */
/*                                                                          */
/*   sdate      Scalar Double          start date inputted                  */
/*                                                                          */
/*   ddot       Scalar Double          annual rate of change of decl.       */
/*                                     (arc-min/yr)                         */
/*                                                                          */
/*   alt        Scalar Double          altitude above WGS84 Ellipsoid       */
/*                                                                          */
/*   epoch      Double array of MAXMOD epoch of model.                      */
/*                                                                          */
/*   ext        Scalar Double          Three 1st-degree external coeff.     */
/*                                                                          */
/*   latitude   Scalar Double          Latitude.                            */
/*                                                                          */
/*   longitude  Scalar Double          Longitude.                           */
/*                                                                          */
/*   gh1        Double array           Schmidt quasi-normal internal        */
/*                                     spherical harmonic coeff.            */
/*                                                                          */
/*   gh2        Double array           Schmidt quasi-normal internal        */
/*                                     spherical harmonic coeff.            */
/*                                                                          */
/*   gha        Double array           Coefficients of resulting model.     */
/*                                                                          */
/*   ghb        Double array           Coefficients of rate of change model.*/
/*                                                                          */
/*   i          Scalar Double          Inclination (deg).                   */
/*                                                                          */
/*   idot       Scalar Double          Rate of change of i (arc-min/yr).    */
/*                                                                          */
/*   igdgc      Integer                Flag for geodetic or geocentric      */
/*                                     coordinate choice.                   */
/*                                                                          */
/*   inbuff     Char a of MAXINBUF     Input buffer.                        */
/*                                                                          */
/*   irec_pos   Integer array of MAXMOD Record counter for header           */
/*                                                                          */
/*   stream  Integer                   File handles for an opened file.     */
/*                                                                          */
/*   fileline   Integer                Current line in file (for errors)    */
/*                                                                          */
/*   max1       Integer array of MAXMOD Main field coefficient.             */
/*                                                                          */
/*   max2       Integer array of MAXMOD Secular variation coefficient.      */
/*                                                                          */
/*   max3       Integer array of MAXMOD Acceleration coefficient.           */
/*                                                                          */
/*   mdfile     Character array of PATH  Model file name.                   */
/*                                                                          */
/*   minyr      Double                  Min year of all models              */
/*                                                                          */
/*   maxyr      Double                  Max year of all models              */
/*                                                                          */
/*   yrmax      Double array of MAXMOD  Max year of model.                  */
/*                                                                          */
/*   yrmin      Double array of MAXMOD  Min year of model.                  */
/*                                                                          */
/****************************************************************************/
void geomag(geomag_vector *gStr) {
    /*  Variable declaration  */

    double sdate = gStr->currentDate;
    double latitude = gStr->latitude;
    double longitude = gStr->longitude;
    double alt = gStr->altitude;

    /* Control variables */
    uint8_t igdgc = 1; /* GEODETIC */
    //uint8_t igdgc = 2; /* GEOCENTRIC */

    static int max1;
    static int max2;
    static int max3;
    static int nmax;

    static uint8_t model[] = "IGRF2020";
    static double epoch;
    static double yrmin;
    static double yrmax;
    static double minyr;
    static double maxyr;
    static double altmin;
    static double altmax;
    static double minalt;
    static double maxalt;

    /*  Obtain the desired model file and read the data  */
    if (PREV_SDATE == 0) {
        epoch = 2020.00;
        max1 = 13;
        max2 = 8;
        max3 = 0;
        yrmin = 2020.00;
        yrmax = 2025.00;
        altmin = -1.0;
        altmax = 600.0;

        minyr = yrmin;
        maxyr = yrmax;
    }

    PREV_SDATE = sdate;
    /* Get altitude min and max for selected model. */
    minalt = altmin;
    maxalt = altmax;
    /* Get Coordinate prefs */
    /* If needed modify ranges to reflect coords. */
    if (igdgc == 2) {
        minalt += 6371.2; /* Add radius to ranges. */
        maxalt += 6371.2;
    }
    /** This will compute everything needed for 1 point in time. **/
    getshc(1, max1, 1);
    getshc(0, max2, 2);
    nmax = extrapsh(sdate, epoch, max1, max2, 3);
    nmax = extrapsh(sdate + 1, epoch, max1, max2, 4);

    /* Do the first calculations */
    shval3(igdgc, latitude, longitude, alt, nmax, 3, IEXT, EXT_COEFF1,
           EXT_COEFF2,
           EXT_COEFF3);
    dihf(3);
    shval3(igdgc, latitude, longitude, alt, nmax, 4, IEXT, EXT_COEFF1,
           EXT_COEFF2,
           EXT_COEFF3);
    dihf(4);
    d = rad2deg(d);
    i = rad2deg(i);

    /* deal with geographic and magnetic poles */
    /* at magnetic poles */
    if (h < 100.0) {
        d = GEO_NAN;
        /* while rest is ok */
    }
    /* at geographic poles */
    if (90.0 - fabs(latitude) <= 0.001) {
        x = GEO_NAN;
        y = GEO_NAN;
        d = GEO_NAN;
        /* while rest is ok */
    }
    /** Above will compute everything for 1 point in time.  **/
    gStr->xMagneticField = x;
    gStr->yMagneticField = y;
    gStr->zMagneticField = z;
    gStr->declination = d;
    gStr->inclination = i;
    gStr->horizontalIntensity = h;
    gStr->totalIntensity = f;
}

/****************************************************************************/
/*                                                                          */
/*                           Subroutine getshc                              */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*     Reads spherical harmonic coefficients from the specified             */
/*     model into an array.                                                 */
/*                                                                          */
/*     Input:                                                               */
/*           stream     - Logical unit number                               */
/*           iflag      - Flag for SV equal to ) or not equal to 0          */
/*                        for designated read statements                    */
/*           strec      - Starting record number to read from model         */
/*           nmax_of_gh - Maximum degree and order of model                 */
/*                                                                          */
/*     Output:                                                              */
/*           gh1 or 2   - Schmidt quasi-normal internal spherical           */
/*                        harmonic coefficients                             */
/*                                                                          */
/*     FORTRAN                                                              */
/*           Bill Flanagan                                                  */
/*           NOAA CORPS, DESDIS, NGDC, 325 Broadway, Boulder CO.  80301     */
/*                                                                          */
/*     C                                                                    */
/*           C. H. Shaffer                                                  */
/*           Lockheed Missiles and Space Company, Sunnyvale CA              */
/*           August 15, 1988                                                */
/*                                                                          */
/****************************************************************************/

static int getshc(int iflag, int nmax_of_gh, int gh) {
    uint8_t irat[] = "IGRF2020";
    int ii, m, n, mm, nn;
    int ios;
    int line_num;
    double g, hh;
    double trash;

    ii = 0;
    ios = 0;
    line_num = 0;

    for (nn = 1; nn <= nmax_of_gh; ++nn) {
        for (mm = 0; mm <= nn; ++mm) {
            if (iflag == 1) {
                n = nn;
                m = mm;
                g = COEFF1[line_num];
                hh = COEFF2[line_num];
                trash = COEFF3[line_num];
                trash = COEFF4[line_num];
                line_num++;
            } else {
                n = nn;
                m = mm;
                trash = COEFF1[line_num];
                trash = COEFF2[line_num];
                g = COEFF3[line_num];
                hh = COEFF4[line_num];
                line_num++;
            }
            if ((nn != n) || (mm != m)) {
                ios = -2;
                return (ios);
            }
            ii = ii + 1;
            switch (gh) {
                case 1:
                    gh1[ii] = g;
                    break;
                case 2:
                    gh2[ii] = g;
                    break;
                default:
                    break;
            }
            if (m != 0) {
                ii = ii + 1;
                switch (gh) {
                    case 1:
                        gh1[ii] = hh;
                        break;
                    case 2:
                        gh2[ii] = hh;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return (ios);
}

/****************************************************************************/
/*                                                                          */
/*                           Subroutine extrapsh                            */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*     Extrapolates linearly a spherical harmonic model with a              */
/*     rate-of-change model.                                                */
/*                                                                          */
/*     Input:                                                               */
/*           date     - date of resulting model (in decimal year)           */
/*           dte1     - date of base model                                  */
/*           nmax1    - maximum degree and order of base model              */
/*           gh1      - Schmidt quasi-normal internal spherical             */
/*                      harmonic coefficients of base model                 */
/*           nmax2    - maximum degree and order of rate-of-change model    */
/*           gh2      - Schmidt quasi-normal internal spherical             */
/*                      harmonic coefficients of rate-of-change model       */
/*                                                                          */
/*     Output:                                                              */
/*           gha or b - Schmidt quasi-normal internal spherical             */
/*                    harmonic coefficients                                 */
/*           nmax   - maximum degree and order of resulting model           */
/*                                                                          */
/*     FORTRAN                                                              */
/*           A. Zunde                                                       */
/*           USGS, MS 964, box 25046 Federal Center, Denver, CO.  80225     */
/*                                                                          */
/*     C                                                                    */
/*           C. H. Shaffer                                                  */
/*           Lockheed Missiles and Space Company, Sunnyvale CA              */
/*           August 16, 1988                                                */
/*                                                                          */
/****************************************************************************/
static int extrapsh(double date, double dte1, int nmax1, int nmax2, int gh) {
    int nmax;
    int k, l;
    int ii;
    double factor;

    factor = date - dte1;
    if (nmax1 == nmax2) {
        k = nmax1 * (nmax1 + 2);
        nmax = nmax1;
    } else {
        if (nmax1 > nmax2) {
            k = nmax2 * (nmax2 + 2);
            l = nmax1 * (nmax1 + 2);
            switch (gh) {
                case 3:
                    for (ii = k + 1; ii <= l; ++ii) {
                        gha[ii] = gh1[ii];
                    }
                    break;
                case 4:
                    for (ii = k + 1; ii <= l; ++ii) {
                        ghb[ii] = gh1[ii];
                    }
                    break;
                default:
                    break;
            }
            nmax = nmax1;
        } else {
            k = nmax1 * (nmax1 + 2);
            l = nmax2 * (nmax2 + 2);
            switch (gh) {
                case 3:
                    for (ii = k + 1; ii <= l; ++ii) {
                        gha[ii] = factor * gh2[ii];
                    }
                    break;
                case 4:
                    for (ii = k + 1; ii <= l; ++ii) {
                        ghb[ii] = factor * gh2[ii];
                    }
                    break;
                default:
                    break;
            }
            nmax = nmax2;
        }
    }
    switch (gh) {
        case 3:
            for (ii = 1; ii <= k; ++ii) {
                gha[ii] = gh1[ii] + factor * gh2[ii];
            }
            break;
        case 4:
            for (ii = 1; ii <= k; ++ii) {
                ghb[ii] = gh1[ii] + factor * gh2[ii];
            }
            break;
        default:
            break;
    }
    return (nmax);
}

/****************************************************************************/
/*                                                                          */
/*                           Subroutine interpsh                            */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*     Interpolates linearly, in time, between two spherical harmonic       */
/*     models.                                                              */
/*                                                                          */
/*     Input:                                                               */
/*           date     - date of resulting model (in decimal year)           */
/*           dte1     - date of earlier model                               */
/*           nmax1    - maximum degree and order of earlier model           */
/*           gh1      - Schmidt quasi-normal internal spherical             */
/*                      harmonic coefficients of earlier model              */
/*           dte2     - date of later model                                 */
/*           nmax2    - maximum degree and order of later model             */
/*           gh2      - Schmidt quasi-normal internal spherical             */
/*                      harmonic coefficients of internal model             */
/*                                                                          */
/*     Output:                                                              */
/*           gha or b - coefficients of resulting model                     */
/*           nmax     - maximum degree and order of resulting model         */
/*                                                                          */
/*     FORTRAN                                                              */
/*           A. Zunde                                                       */
/*           USGS, MS 964, box 25046 Federal Center, Denver, CO.  80225     */
/*                                                                          */
/*     C                                                                    */
/*           C. H. Shaffer                                                  */
/*           Lockheed Missiles and Space Company, Sunnyvale CA              */
/*           August 17, 1988                                                */
/*                                                                          */
/****************************************************************************/
static int interpsh(double date, double dte1, int nmax1, double dte2, int nmax2,
                    int gh) {
    int nmax;
    int k, l;
    int ii;
    double factor;

    factor = (date - dte1) / (dte2 - dte1);
    if (nmax1 == nmax2) {
        k = nmax1 * (nmax1 + 2);
        nmax = nmax1;
    } else {
        if (nmax1 > nmax2) {
            k = nmax2 * (nmax2 + 2);
            l = nmax1 * (nmax1 + 2);
            switch (gh) {
                case 3:
                    for (ii = k + 1; ii <= l; ++ii) {
                        gha[ii] = gh1[ii] + factor * (-gh1[ii]);
                    }
                    break;
                case 4:
                    for (ii = k + 1; ii <= l; ++ii) {
                        ghb[ii] = gh1[ii] + factor * (-gh1[ii]);
                    }
                    break;
                default:
                    break;
            }
            nmax = nmax1;
        } else {
            k = nmax1 * (nmax1 + 2);
            l = nmax2 * (nmax2 + 2);
            switch (gh) {
                case 3:
                    for (ii = k + 1; ii <= l; ++ii) {
                        gha[ii] = factor * gh2[ii];
                    }
                    break;
                case 4:
                    for (ii = k + 1; ii <= l; ++ii) {
                        ghb[ii] = factor * gh2[ii];
                    }
                    break;
                default:
                    break;
            }
            nmax = nmax2;
        }
    }
    switch (gh) {
        case 3:
            for (ii = 1; ii <= k; ++ii) {
                gha[ii] = gh1[ii] + factor * (gh2[ii] - gh1[ii]);
            }
            break;
        case 4:
            for (ii = 1; ii <= k; ++ii) {
                ghb[ii] = gh1[ii] + factor * (gh2[ii] - gh1[ii]);
            }
            break;
        default:
            break;
    }
    return (nmax);
}

/****************************************************************************/
/*                                                                          */
/*                           Subroutine shval3                              */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*     Calculates field components from spherical harmonic (sh)             */
/*     models.                                                              */
/*                                                                          */
/*     Input:                                                               */
/*           igdgc     - indicates coordinate system used; set equal        */
/*                       to 1 if geodetic, 2 if geocentric                  */
/*           latitude  - north latitude, in degrees                         */
/*           longitude - east longitude, in degrees                         */
/*           elev      - WGS84 altitude above ellipsoid (igdgc=1), or       */
/*                       radial distance from earth's center (igdgc=2)      */
/*           a2,b2     - squares of semi-major and semi-minor axes of       */
/*                       the reference spheroid used for transforming       */
/*                       between geodetic and geocentric coordinates        */
/*                       or components                                      */
/*           nmax      - maximum degree and order of coefficients           */
/*           iext      - external coefficients flag (=0 if none)            */
/*           ext1,2,3  - the three 1st-degree external coefficients         */
/*                       (not used if iext = 0)                             */
/*                                                                          */
/*     Output:                                                              */
/*           x         - northward component                                */
/*           y         - eastward component                                 */
/*           z         - vertically-downward component                      */
/*                                                                          */
/*     based on subroutine 'igrf' by D. R. Barraclough and S. R. C. Malin,  */
/*     report no. 71/1, institute of geological sciences, U.K.              */
/*                                                                          */
/*     FORTRAN                                                              */
/*           Norman W. Peddie                                               */
/*           USGS, MS 964, box 25046 Federal Center, Denver, CO.  80225     */
/*                                                                          */
/*     C                                                                    */
/*           C. H. Shaffer                                                  */
/*           Lockheed Missiles and Space Company, Sunnyvale CA              */
/*           August 17, 1988                                                */
/*                                                                          */
/****************************************************************************/
static int shval3(uint8_t igdgc, double flat, double flon, double elev, int nmax,
                  int gh, int iext, double ext1, double ext2, double ext3) {
    double earths_radius = 6371.2;
    double dtr = 0.01745329;
    double slat;
    double clat;
    double ratio;
    double aa, bb, cc, dd;
    double sd;
    double cd;
    double r;
    double a2;
    double b2;
    double rr;
    double fm, fn;
    double sl[14];
    double cl[14];
    double p[119];
    double q[119];
    int ii, j, k, l, m, n;
    int npq;
    int ios;
    double argument;
    double power;
    a2 = 40680631.59; /* WGS84 */
    b2 = 40408299.98; /* WGS84 */
    ios = 0;
    r = elev;
    argument = flat * dtr;
    slat = sin(argument);
    if ((90.0 - flat) < 0.001) {
        aa = 89.999; /*  300 ft. from North pole  */
    } else {
        if ((90.0 + flat) < 0.001) {
            aa = -89.999; /*  300 ft. from South pole  */
        } else {
            aa = flat;
        }
    }
    argument = aa * dtr;
    clat = cos(argument);
    argument = flon * dtr;
    sl[1] = sin(argument);
    cl[1] = cos(argument);
    switch (gh) {
        case 3:
            x = 0;
            y = 0;
            z = 0;
            break;
        case 4:
            xtemp = 0;
            ytemp = 0;
            ztemp = 0;
            break;
        default:
            break;
    }
    sd = 0.0;
    cd = 1.0;
    l = 1;
    n = 0;
    m = 1;
    npq = (nmax * (nmax + 3)) / 2;
    if (igdgc == 1) {
        aa = a2 * clat * clat;
        bb = b2 * slat * slat;
        cc = aa + bb;
        argument = cc;
        dd = sqrt(argument);
        argument = elev * (elev + 2.0 * dd) + (a2 * aa + b2 * bb) / cc;
        r = sqrt(argument);
        cd = (elev + dd) / r;
        sd = (a2 - b2) / dd * slat * clat / r;
        aa = slat;
        slat = slat * cd - clat * sd;
        clat = clat * cd + aa * sd;
    }
    ratio = earths_radius / r;
    argument = 3.0;
    aa = sqrt(argument);
    p[1] = 2.0 * slat;
    p[2] = 2.0 * clat;
    p[3] = 4.5 * slat * slat - 1.5;
    p[4] = 3.0 * aa * clat * slat;
    q[1] = -clat;
    q[2] = slat;
    q[3] = -3.0 * clat * slat;
    q[4] = aa * (slat * slat - clat * clat);
    for (k = 1; k <= npq; ++k) {
        if (n < m) {
            m = 0;
            n = n + 1;
            argument = ratio;
            power = n + 2;
            rr = pow(argument, power);
            fn = n;
        }
        fm = m;
        if (k >= 5) {
            if (m == n) {
                argument = (1.0 - 0.5 / fm);
                aa = sqrt(argument);
                j = k - n - 1;
                p[k] = (1.0 + 1.0 / fm) * aa * clat * p[j];
                q[k] = aa * (clat * q[j] + slat / fm * p[j]);
                sl[m] = sl[m - 1] * cl[1] + cl[m - 1] * sl[1];
                cl[m] = cl[m - 1] * cl[1] - sl[m - 1] * sl[1];
            } else {
                argument = fn * fn - fm * fm;
                aa = sqrt(argument);
                argument = ((fn - 1.0) * (fn - 1.0)) - (fm * fm);
                bb = sqrt(argument) / aa;
                cc = (2.0 * fn - 1.0) / aa;
                ii = k - n;
                j = k - 2 * n + 1;
                p[k] = (fn + 1.0)
                       * (cc * slat / fn * p[ii] - bb / (fn - 1.0) * p[j]);
                q[k] = cc * (slat * q[ii] - clat / fn * p[ii]) - bb * q[j];
            }
        }
        switch (gh) {
            case 3:
                aa = rr * gha[l];
                break;
            case 4:
                aa = rr * ghb[l];
                break;
            default:
                break;
        }
        if (m == 0) {
            switch (gh) {
                case 3:
                    x = x + aa * q[k];
                    z = z - aa * p[k];
                    break;
                case 4:
                    xtemp = xtemp + aa * q[k];
                    ztemp = ztemp - aa * p[k];
                    break;
                default:
                    break;
            }
            l = l + 1;
        } else {
            switch (gh) {
                case 3:
                    bb = rr * gha[l + 1];
                    cc = aa * cl[m] + bb * sl[m];
                    x = x + cc * q[k];
                    z = z - cc * p[k];
                    if (clat > 0) {
                        y = y
                            + (aa * sl[m] - bb * cl[m]) * fm * p[k]
                              / ((fn + 1.0) * clat);
                    } else {
                        y = y + (aa * sl[m] - bb * cl[m]) * q[k] * slat;
                    }
                    l = l + 2;
                    break;
                case 4:
                    bb = rr * ghb[l + 1];
                    cc = aa * cl[m] + bb * sl[m];
                    xtemp = xtemp + cc * q[k];
                    ztemp = ztemp - cc * p[k];
                    if (clat > 0) {
                        ytemp = ytemp
                                + (aa * sl[m] - bb * cl[m]) * fm * p[k]
                                  / ((fn + 1.0) * clat);
                    } else {
                        ytemp = ytemp + (aa * sl[m] - bb * cl[m]) * q[k] * slat;
                    }
                    l = l + 2;
                    break;
                default:
                    break;
            }
        }
        m = m + 1;
    }
    if (iext != 0) {
        aa = ext2 * cl[1] + ext3 * sl[1];
        switch (gh) {
            case 3:
                x = x - ext1 * clat + aa * slat;
                y = y + ext2 * sl[1] - ext3 * cl[1];
                z = z + ext1 * slat + aa * clat;
                break;
            case 4:
                xtemp = xtemp - ext1 * clat + aa * slat;
                ytemp = ytemp + ext2 * sl[1] - ext3 * cl[1];
                ztemp = ztemp + ext1 * slat + aa * clat;
                break;
            default:
                break;
        }
    }
    switch (gh) {
        case 3:
            aa = x;
            x = x * cd + z * sd;
            z = z * cd - aa * sd;
            break;
        case 4:
            aa = xtemp;
            xtemp = xtemp * cd + ztemp * sd;
            ztemp = ztemp * cd - aa * sd;
            break;
        default:
            break;
    }
    return (ios);
}

/****************************************************************************/
/*                                                                          */
/*                           Subroutine dihf                                */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*     Computes the geomagnetic d, i, h, and f from x, y, and z.            */
/*                                                                          */
/*     Input:                                                               */
/*           x  - northward component                                       */
/*           y  - eastward component                                        */
/*           z  - vertically-downward component                             */
/*                                                                          */
/*     Output:                                                              */
/*           d  - declination                                               */
/*           i  - inclination                                               */
/*           h  - horizontal intensity                                      */
/*           f  - total intensity                                           */
/*                                                                          */
/*     FORTRAN                                                              */
/*           A. Zunde                                                       */
/*           USGS, MS 964, box 25046 Federal Center, Denver, CO.  80225     */
/*                                                                          */
/*     C                                                                    */
/*           C. H. Shaffer                                                  */
/*           Lockheed Missiles and Space Company, Sunnyvale CA              */
/*           August 22, 1988                                                */
/*                                                                          */
/****************************************************************************/
static int dihf(int gh) {
    int ios;
    int j;
    double sn;
    double h2;
    double hpx;
    double argument, argument2;

    ios = gh;
    sn = 0.0001;

    switch (gh) {
        case 3:
            for (j = 1; j <= 1; ++j) {
                h2 = x * x + y * y;
                argument = h2;
                h = sqrt(argument); /* calculate horizontal intensity */
                argument = h2 + z * z;
                f = sqrt(argument); /* calculate total intensity */
                if (f < sn) {
                    d = GEO_NAN; /* If d and i cannot be determined, */
                    i = GEO_NAN; /*       set equal to GEO_NAN         */
                } else {
                    argument = z;
                    argument2 = h;
                    i = atan2(argument, argument2);
                    if (h < sn) {
                        d = GEO_NAN;
                    } else {
                        hpx = h + x;
                        if (hpx < sn) {
                            d = PI;
                        } else {
                            argument = y;
                            argument2 = hpx;
                            d = 2.0 * atan2(argument, argument2);
                        }
                    }
                }
            }
            break;
        case 4:
            for (j = 1; j <= 1; ++j) {
                h2 = xtemp * xtemp + ytemp * ytemp;
                argument = h2;
                htemp = sqrt(argument);
                argument = h2 + ztemp * ztemp;
                ftemp = sqrt(argument);
                if (ftemp < sn) {
                    dtemp = GEO_NAN; /* If d and i cannot be determined, */
                    itemp = GEO_NAN; /*       set equal to 999.0         */
                } else {
                    argument = ztemp;
                    argument2 = htemp;
                    itemp = atan2(argument, argument2);
                    if (htemp < sn) {
                        dtemp = GEO_NAN;
                    } else {
                        hpx = htemp + xtemp;
                        if (hpx < sn) {
                            dtemp = PI;
                        } else {
                            argument = ytemp;
                            argument2 = hpx;
                            dtemp = 2.0 * atan2(argument, argument2);
                        }
                    }
                }
            }
            break;
        default:
            break;
    }
    return (ios);
}