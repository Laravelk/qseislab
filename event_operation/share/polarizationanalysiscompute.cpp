#include "polarizationanalysiscompute.h"
#include "data/seismevent.h"

#include <QtMath>

#include <iostream> // TODO: delete

namespace EventOperation {
PolarizationAnalysisCompute::PolarizationAnalysisCompute(Data::SeismEvent * const event)
    : _event(event) {}

void PolarizationAnalysisCompute::calculate() {
//  _event = event.get();
  int numberOfComponent = 0;
  for (auto &component : _event->getComponents()) {
    for (auto &mapsWithPickElement : component->getWavePicks()) {
      Data::SeismWavePick &pick = mapsWithPickElement.second;
      test_data_for_print_type = pick.getType();
      _oldDataMap[std::make_pair(numberOfComponent, pick.getType())] =
          pick.getPolarizationAnalysisData();
      const int FIRST_NUMBER_OF_ELEMENT_IN_ARRAY = static_cast<const int>(
          pick.getPolarizationLeftBorder() / component->getSampleInterval());
      const int LAST_NUMBER_OF_ELEMENT_IN_ARRAY =
          static_cast<const int>((pick.getPolarizationRightBorder() + 0.99f) /
                                 component->getSampleInterval());
      Eigen::MatrixXf matrix =
          getPointMatrix(component, FIRST_NUMBER_OF_ELEMENT_IN_ARRAY,
                         LAST_NUMBER_OF_ELEMENT_IN_ARRAY);
      Data::SeismPolarizationAnalysisData data =
          calculatePolarizationData(matrix);
      data.setValid(true);
      pick.setPolarizationAnalysisData(data);
      _currentlyMap[std::make_pair(numberOfComponent, pick.getType())] =
              pick.getPolarizationAnalysisData();
    }
    ++numberOfComponent;
    test_data_for_print_num_of_comp = numberOfComponent;
  }
}

// void PolarizationAnalysisCompute::undo()
//{
//    int componentNumber = 0;
//    for (auto &component : _event->getComponents()) {
//        for (auto &mapsWithPickElement : component->getWavePicks()) {
//            Data::SeismWavePick pick = mapsWithPickElement.second;
//            std::optional<Data::SeismPolarizationAnalysisData*> optionalData =
//                    _oldDataMap.at(std::make_pair(componentNumber,
//                    pick.getType()));
//            pick.setPolarizationAnalysisData(optionalData);
//        }
//        componentNumber++;
//    }
//}

// void PolarizationAnalysisCompute::redo()
//{
//    calculate();
//}

Eigen::MatrixXf PolarizationAnalysisCompute::getPointMatrix(
    Data::SeismComponent *const component, int firstIndex, int lastIndex) {
  const float MAX_VALUE = component->getMaxValue();
  Eigen::MatrixXf pointMatrix(3, lastIndex - firstIndex);
  auto bufferX = component->getTraces().at(0)->getBuffer();
  auto bufferY = component->getTraces().at(1)->getBuffer();
  auto bufferZ = component->getTraces().at(2)->getBuffer();

  Eigen::RowVectorXf vectorX(lastIndex - firstIndex);
  Eigen::RowVectorXf vectorY(lastIndex - firstIndex);
  Eigen::RowVectorXf vectorZ(lastIndex - firstIndex);

  for (int i  = firstIndex;
       i < lastIndex; i++) {
      vectorX[i - firstIndex] = bufferX[i] / MAX_VALUE;
      vectorY[i - firstIndex] = bufferY[i] / MAX_VALUE;
      vectorZ[i - firstIndex] = bufferZ[i] / MAX_VALUE;
  }

  pointMatrix << vectorX, vectorY, vectorZ;
  return pointMatrix;
}

Data::SeismPolarizationAnalysisData
PolarizationAnalysisCompute::calculatePolarizationData(
    const Eigen::MatrixXf &matrix) {
  Eigen::BDCSVD<Eigen::MatrixXf> *svd = new Eigen::BDCSVD<Eigen::MatrixXf>(
      matrix, Eigen::ComputeFullV | Eigen::ComputeFullU);
  Eigen::Vector3f vectorWithTheBiggestEigenValue, column2, column3;
  vectorWithTheBiggestEigenValue = svd->matrixU().col(0);

  QVector3D eigenVector(vectorWithTheBiggestEigenValue[0], vectorWithTheBiggestEigenValue[1], vectorWithTheBiggestEigenValue[2]);

  if (vectorWithTheBiggestEigenValue[2] < 0) {
//      std::cerr << "EXIST IN POLAR COMPUTE " << std::endl;
      eigenVector *= -1;
  }

  const double maxSingularValue = static_cast<double>(svd->singularValues()[0]);
  const double pAzimutInRadian =
      static_cast<double>(std::atan(vectorWithTheBiggestEigenValue[1] * sgn(vectorWithTheBiggestEigenValue[2]) /
                          (vectorWithTheBiggestEigenValue[0] * sgn(vectorWithTheBiggestEigenValue[2]))));
  const double pIncidenceInRadian =
      static_cast<double>(std::acos(qAbs(vectorWithTheBiggestEigenValue[2])));

  const double pAzimutDegrees = pAzimutInRadian * DEGREES_COEFFICIENT / M_PI;
  const double pIncidenceDegrees =
      pIncidenceInRadian * DEGREES_COEFFICIENT / M_PI;

//  std::cerr << "Polar: " << pAzimutDegrees << " number " << test_data_for_print_num_of_comp << " Type " << test_data_for_print_type << std::endl;
//  std::cerr << vectorWithTheBiggestEigenValue[0] << " " << vectorWithTheBiggestEigenValue[1] << " " << vectorWithTheBiggestEigenValue[2] <<std::endl << std::endl;
  return Data::SeismPolarizationAnalysisData(
      maxSingularValue, pAzimutInRadian, pIncidenceInRadian, pAzimutDegrees,
      pIncidenceDegrees, eigenVector);
}

} // namespace EventOperation
