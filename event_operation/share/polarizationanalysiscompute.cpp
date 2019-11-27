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
      _oldDataMap[std::make_pair(numberOfComponent, pick.getType())] =
          pick.getPolarizationAnalysisData();
      const int FIRST_NUMBER_OF_ELEMENT_IN_ARRAY = static_cast<const int>(
          pick.getPolarizationLeftBorder() / component->getSampleInterval());
      const int LAST_NUMBER_OF_ELEMENT_IN_ARRAY =
          static_cast<const int>((pick.getPolarizationRightBorder() + 0.99f) /
                                 component->getSampleInterval());
      Eigen::MatrixXf *matrix =
          getPointMatrix(component, FIRST_NUMBER_OF_ELEMENT_IN_ARRAY,
                         LAST_NUMBER_OF_ELEMENT_IN_ARRAY);
      Data::SeismPolarizationAnalysisData *data =
          calculatePolarizationData(*matrix);
      data->setValid(true);
//      component->removeWavePick(pick.getType());
      pick.setPolarizationAnalysisData(data);
      std::cerr << pick.getType();

//      component->addWavePick(pick);
    }
    ++numberOfComponent;
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

Eigen::MatrixXf *PolarizationAnalysisCompute::getPointMatrix(
    Data::SeismComponent *const component, int firstIndex, int lastIndex) {
  const float MAX_VALUE = component->getMaxValue();
  Eigen::MatrixXf *pointMatrix = new Eigen::MatrixXf(3, lastIndex - firstIndex);
  auto bufferX = component->getTraces().at(0)->getBuffer();
  auto bufferY = component->getTraces().at(1)->getBuffer();
  auto bufferZ = component->getTraces().at(2)->getBuffer();

  for (unsigned long i = static_cast<unsigned long>(firstIndex), rowNumber = 0;
       i < static_cast<unsigned long>(lastIndex); i++, rowNumber++) {
    (*pointMatrix)(0, static_cast<long>(rowNumber)) = bufferX[i] / MAX_VALUE;
    (*pointMatrix)(1, static_cast<long>(rowNumber)) = bufferY[i] / MAX_VALUE;
    (*pointMatrix)(2, static_cast<long>(rowNumber)) = bufferZ[i] / MAX_VALUE;
  }
  return pointMatrix;
}

Data::SeismPolarizationAnalysisData *
PolarizationAnalysisCompute::calculatePolarizationData(
    const Eigen::MatrixXf &matrix) {
  Eigen::BDCSVD<Eigen::MatrixXf> *svd = new Eigen::BDCSVD<Eigen::MatrixXf>(
      matrix, Eigen::ComputeFullV | Eigen::ComputeFullU);
  Eigen::Vector3f column1, column2, column3;
  column1 = svd->matrixU().col(0);
  column2 = svd->matrixU().col(1);
  column3 = svd->matrixU().col(2);

  const double maxSingularValue = static_cast<double>(svd->singularValues()[0]);
  const double pAzimutInRadian =
      static_cast<double>(std::atan((column1[1] * sgn(column1[0]))) /
                          (column1[2] * sgn(column1[0])));
  const double pIncidenceInRadian =
      static_cast<double>(std::acos(qAbs(column1[0])));

  const double pAzimutDegrees = pAzimutInRadian * DEGREES_COEFFICIENT / M_PI;
  const double pIncidenceDegrees =
      pIncidenceInRadian * DEGREES_COEFFICIENT / M_PI;

  return new Data::SeismPolarizationAnalysisData(
      maxSingularValue, pAzimutInRadian, pIncidenceInRadian, pAzimutDegrees,
      pIncidenceDegrees);
}

} // namespace EventOperation
