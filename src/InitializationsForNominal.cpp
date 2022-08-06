#include "InitializationsForNominal.hpp"
#include "Definitions.hpp"

using namespace Eigen;
using namespace Parameters::CovarianceMatrices;

PAndReflectivityData InitializationsForNominal(){
    PAndReflectivityData refDataAndP;
    Matrix<float, LocalStateSize, LocalStateSize> P;
    auto reflectivityData = EarthCellsMatrix::Identity() * 0;
    P << 0.00136699495713899, -0.000114760060275635, 0.000256861463917196, 0, 0, 0,
            -0.000114760060275612, 0.000441521240017950, -7.67733697177619e-06, 0, 0, 0,
            0.000256861463917308, -7.67733697184630e-06, 0.000401588226538019, 0, 0, 0,
            0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 1;
    refDataAndP.P = P;
    refDataAndP.reflectivityData = reflectivityData;
    OrbitalParameters orbitalParameters;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    refDataAndP.orbitalParameters = orbitalParameters;
    return refDataAndP;
};

