#include "svdmath.h"

#include <QtMath>

namespace EventMath {

Data::SeismPolarizationAnalysisData SVDMath::calculatePolarizationData(const Eigen::MatrixXf &matrix)
{
    Eigen::BDCSVD<Eigen::MatrixXf> *svd = new Eigen::BDCSVD<Eigen::MatrixXf>(
        matrix, Eigen::ComputeFullV | Eigen::ComputeFullU);
    Eigen::Vector3f vectorWithTheBiggestEigenValue, column2, column3;
    vectorWithTheBiggestEigenValue = svd->matrixU().col(0);

    auto singularValues = svd->singularValues();

    QVector3D eigenVector(vectorWithTheBiggestEigenValue[0],
                          vectorWithTheBiggestEigenValue[1],
                          vectorWithTheBiggestEigenValue[2]);

    if (vectorWithTheBiggestEigenValue[2] < 0) {
        eigenVector *= -1;
    }

    const double maxSingularValue = static_cast<double>(svd->singularValues()[0]);
    const double pAzimutInRadian =
        static_cast<double>(std::atan(vectorWithTheBiggestEigenValue[1] *
                                      sgn(vectorWithTheBiggestEigenValue[2]) /
                                      (vectorWithTheBiggestEigenValue[0] *
                                       sgn(vectorWithTheBiggestEigenValue[2]))));
    const double pIncidenceInRadian =
        static_cast<double>(std::acos(qAbs(vectorWithTheBiggestEigenValue[2])));

    const double pAzimutDegrees = pAzimutInRadian * DEGREES_COEFFICIENT / M_PI;
    const double pIncidenceDegrees =
        pIncidenceInRadian * DEGREES_COEFFICIENT / M_PI;

    const float degree_of_rectilinearity =
        1.0f -
        ((singularValues[0] + singularValues[2]) / (2 * singularValues[1]));
    const float degree_of_planarity =
        1.0f - (2 * singularValues[2] / (singularValues[0] + singularValues[1]));

    //  std::cerr << "Polar: " << pAzimutDegrees << " number " <<
    //  test_data_for_print_num_of_comp << " Type " << test_data_for_print_type <<
    //  std::endl;
    //  std::cerr << vectorWithTheBiggestEigenValue[0] << " " <<
    //  vectorWithTheBiggestEigenValue[1] << " " <<
    //  vectorWithTheBiggestEigenValue[2] <<std::endl << std::endl;
    return Data::SeismPolarizationAnalysisData(
        maxSingularValue, pAzimutInRadian, pIncidenceInRadian, pAzimutDegrees,
        pIncidenceDegrees, degree_of_planarity, degree_of_rectilinearity,
        eigenVector);
}

}
