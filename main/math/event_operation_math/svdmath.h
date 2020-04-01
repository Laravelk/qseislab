#pragma once

#include <Eigen/Eigenvalues>
#include <Eigen/Jacobi>
#include <Eigen/SVD>

#include "data/seismpolarizationanalysisdata.h"

namespace Data {
class SeismEvent;
class SeismPolarizationAnalysisData;
}

namespace EventMath {
class SVDMath
{
public:
    static Data::SeismPolarizationAnalysisData
    calculatePolarizationData(const Eigen::MatrixXf &);

private:
    SVDMath(){}
    static int sgn(double val) { return (0 < val) - (val < 0); }
    constexpr static const double DEGREES_COEFFICIENT = 180;
};
}
