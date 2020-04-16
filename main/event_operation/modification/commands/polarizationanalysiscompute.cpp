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

      //      std::cerr << "THIS COMPONENT IS: " << numberOfComponent << " ";

      Eigen::MatrixXf matrix =
          getPointMatrix(component, FIRST_NUMBER_OF_ELEMENT_IN_ARRAY,
                         LAST_NUMBER_OF_ELEMENT_IN_ARRAY);
      //      std::cerr << "MATRIX " << std::endl << matrix << std::endl;
      Data::SeismPolarizationAnalysisData data =
          EventMath::SVDMath::calculatePolarizationData(matrix);
      //      std::cerr << "DATA: " << std::endl;
      //      data.print();
      // calculatePolarizationData(matrix);
      data.setValid(true);
      pick.setPolarizationAnalysisData(data);
    }
    ++numberOfComponent;
  }
}

void PolarizationAnalysisCompute::undoForOne(Data::SeismEvent *event) {
  int componentNumber = 0;
  for (auto &component : event->getComponents()) {
    for (auto &mapsWithPickElement : component->getWavePicks()) {
      std::optional<Data::SeismPolarizationAnalysisData> optionalData =
          _oldDataMap.at(std::make_pair(componentNumber,
                                        mapsWithPickElement.second.getType()));
      mapsWithPickElement.second.setPolarizationAnalysisData(optionalData);
    }
    componentNumber++;
  }
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
  //  std::cerr << "MAX VALUE: " << MAX_VALUE << std::endl;
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

  //  std::cerr << "First index: " << firstIndex << " and last: " << lastIndex
  //            << std::endl;
  //  std::cerr << "X: " << std::endl;
  //  for (int i = firstIndex; i < lastIndex; i++) {
  //    std::cerr << bufferX[i] << ", ";
  //  }

  //  std::cerr << std::endl << std::endl;
  //  std::cerr << "Y: " << std::endl;
  //  for (int i = firstIndex; i < lastIndex; i++) {
  //    std::cerr << bufferY[i] << ", ";
  //  }
  //  std::cerr << std::endl << std::endl;
  //  std::cerr << "Z: " << std::endl;
  //  for (int i = firstIndex; i < lastIndex; i++) {
  //    std::cerr << bufferZ[i] << ", ";
  //  }
  //  std::cerr << std::endl << std::endl;
  pointMatrix << vectorX, vectorY, vectorZ;
  return pointMatrix;
}
