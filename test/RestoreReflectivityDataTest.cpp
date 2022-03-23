#include <catch2/catch.hpp>
#include "RestoreReflectivityData.hpp"

using namespace Eigen;

TEST_CASE("Restore Reflectivity Data algorithm test (random matrix)"){
    Matrix<int32_t , ReflectivityDataRows, ReflectivityDataColumns> compressedReflectivityData = Matrix<int32_t , ReflectivityDataRows, ReflectivityDataColumns>::Identity() * -53338;
    EarthCellsMatrix decompressedReflectivityData = recoverData(compressedReflectivityData,0.69,exp(0.69));
    REQUIRE(decompressedReflectivityData.sum() == Approx(103119.544997582).epsilon(1e-4));
}