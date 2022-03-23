#include "RestoreReflectivityData.hpp"

using namespace Eigen;

EarthCellsMatrix recoverData(Matrix<int32_t , ReflectivityDataRows, ReflectivityDataColumns> compressedReflectivityData,float minimumOfTheOriginalMAtrix ,float maximumOfTheOriginalMAtrix){
    EarthCellsMatrix reflectivityData;
    for (int i = 0; i<ReflectivityDataRows ; i++){
        for (int j = 0; j<ReflectivityDataColumns;j++){
            reflectivityData(i,j)= exp(float(compressedReflectivityData(i,j))/65536 * (maximumOfTheOriginalMAtrix - minimumOfTheOriginalMAtrix) + minimumOfTheOriginalMAtrix);
        }
    }
    return reflectivityData;
};
