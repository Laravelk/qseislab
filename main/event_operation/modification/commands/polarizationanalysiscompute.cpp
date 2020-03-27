#include "polarizationanalysiscompute.h"
#include "data/seismevent.h"

#include <QtMath>

#include <iostream> // TODO: delete

PolarizationAnalysisCompute::PolarizationAnalysisCompute(
    const std::set<Data::SeismEvent *> &events, const Parameters &)
    : EventOperationUndoCommand(events) {}

void PolarizationAnalysisCompute::calculate() {
  int numberOfComponent = 0;
  for (auto &component : _event->getComponents()) {
    for (auto &mapsWithPickElement : component->getWavePicks()) {
      Data::SeismWavePick &pick = mapsWithPickElement.second;
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
    }
    ++numberOfComponent;
  }
}

void PolarizationAnalysisCompute::undoForOne(Data::SeismEvent *event) {
  //  int componentNumber = 0;
  //  std::cerr << "start undo" << std::endl;
  //  for (auto &component : event->getComponents()) {
  //    for (auto &mapsWithPickElement : component->getWavePicks()) {
  //      std::optional<Data::SeismPolarizationAnalysisData> optionalData =
  //          _oldDataMap.at(std::make_pair(componentNumber,
  //                                        mapsWithPickElement.second.getType()));
  //      mapsWithPickElement.second.setPolarizationAnalysisData(optionalData);
  //    }
  //    componentNumber++;
  //  }
  //  std::cerr << "end undo" << std::endl;
  if (!_disableSignal) {
    event->changeTrigger();
  }
}

void PolarizationAnalysisCompute::redoForOne(Data::SeismEvent *event) {
  _event = event;
  calculate();
  if (!_disableSignal) {
    event->changeTrigger();
  }
}

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

  for (int i = firstIndex; i < lastIndex; i++) {
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
