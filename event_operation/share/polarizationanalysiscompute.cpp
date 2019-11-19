#include "polarizationanalysisdata.h"
#include "data/seismevent.h"

#include <QtMath>

namespace EventOperation {

PolarizationAnalysisData::PolarizationAnalysisData(
    std::unique_ptr<Data::SeismEvent> &event)
    : _event(event.get()) {}

bool PolarizationAnalysisData::calculatePolarizationData() {
  if (!_isValid) {
    int index = 0;
    for (auto &component : _event->getComponents()) {
      if (_numberComponent == index) {
        Data::SeismWavePick pick = component->getWavePick(_type);
        const int FIRST_NUMBER_OF_ELEMENT_IN_ARRAY = static_cast<const int>(
            pick.getPolarizationLeftBorder() / component->getSampleInterval());
        const int LAST_NUMBER_OF_ELEMENT_IN_ARRAY =
            static_cast<const int>(pick.getPolarizationRightBorder() +
                                   0.99f / component->getSampleInterval());
        Eigen::MatrixXf *matrix =
            getPointMatrix(component, FIRST_NUMBER_OF_ELEMENT_IN_ARRAY,
                           LAST_NUMBER_OF_ELEMENT_IN_ARRAY);
        calculateSVD(*matrix);
        return true;
      }
    }
  }
  return false;
}

Eigen::MatrixXf *PolarizationAnalysisData::getPointMatrix(
    const std::unique_ptr<Data::SeismComponent> &component, int firstIndex,
    int lastIndex) {
  const float MAX_VALUE = component->getMaxValue();
  Eigen::MatrixXf *pointMatrix = new Eigen::MatrixXf(3, lastIndex - firstIndex);
  const std::unique_ptr<float[]> &bufferX =
      component->getTraces().at(0)->getBuffer();
  const std::unique_ptr<float[]> &bufferY =
      component->getTraces().at(1)->getBuffer();
  const std::unique_ptr<float[]> &bufferZ =
      component->getTraces().at(2)->getBuffer();

  for (unsigned long i = static_cast<unsigned long>(firstIndex), rowNumber = 0;
       i < static_cast<unsigned long>(lastIndex); i++, rowNumber++) {
    (*pointMatrix)(0, static_cast<long>(rowNumber)) = bufferX[i] / MAX_VALUE;
    (*pointMatrix)(1, static_cast<long>(rowNumber)) = bufferY[i] / MAX_VALUE;
    (*pointMatrix)(2, static_cast<long>(rowNumber)) = bufferZ[i] / MAX_VALUE;
  }
  return pointMatrix;
}

void PolarizationAnalysisData::calculateSVD(const Eigen::MatrixXf &matrix) {
  Eigen::BDCSVD<Eigen::MatrixXf> *svd = new Eigen::BDCSVD<Eigen::MatrixXf>(
      matrix, Eigen::ComputeFullV | Eigen::ComputeFullU);
  Eigen::Vector3f column1, column2, column3;
  column1 = svd->matrixU().col(0);
  column2 = svd->matrixU().col(1);
  column3 = svd->matrixU().col(2);

  _pAzimutInRadian = std::atan((column1[1] * sgn(column1[0])) /
                               (column1[2] * sgn(column1[0])));
  _pIncidenceInRadian = std::acos(qAbs(column1[0]));

  _pAzimutDegrees = _pAzimutInRadian * 180 / M_PI;
  _pIncidenceDegrees = _pIncidenceInRadian * 180 / M_PI;
}

} // namespace EventOperation
