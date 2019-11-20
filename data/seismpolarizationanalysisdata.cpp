#include "seismpolarizationanalysisdata.h"

#include <QtMath>

namespace Data {

SeismPolarizationAnalysisData::SeismPolarizationAnalysisData(double maxSingularValue, double pAzimutInRadian,
                                                             double pIncidenceInRadian, double pAzimutDegrees,
                                                             double pIncidenceDegrees)
    : _maxSingularValue(maxSingularValue), _pAzimutInRadian(pAzimutInRadian), _pIncidenceInRadian(pIncidenceInRadian),
            _pAzimutDegrees(pAzimutDegrees), _pIncidenceDegrees(pIncidenceDegrees) {}

void SeismPolarizationAnalysisData::setValid(const bool validStatus) {
  _isValid = validStatus;
}

void SeismPolarizationAnalysisData::setMaxSingularVale(const double value) {
  _maxSingularValue = value;
}

void SeismPolarizationAnalysisData::setAzimutInRadian(const double radian) {
  _pAzimutInRadian = radian;
  _pAzimutDegrees = radian * DEGREES_COEFFICIENT / M_PI;
}

void SeismPolarizationAnalysisData::setIncidenceInRadian(const double radian) {
  _pIncidenceInRadian = radian;
  _pAzimutDegrees = radian * DEGREES_COEFFICIENT / M_PI;
}

void SeismPolarizationAnalysisData::setAzimutDegrees(const double degrees) {
  _pAzimutDegrees = degrees;
  _pAzimutInRadian = degrees * M_PI / DEGREES_COEFFICIENT;
}

void SeismPolarizationAnalysisData::setIncidenceDegrees(const double degrees) {
  _pIncidenceDegrees = degrees;
  _pIncidenceInRadian = degrees * M_PI / DEGREES_COEFFICIENT;
}

bool SeismPolarizationAnalysisData::isValid() const { return _isValid; }

double SeismPolarizationAnalysisData::getMaxSingularValue() const {
  return _maxSingularValue;
}

double SeismPolarizationAnalysisData::getAzimutInRadian() const {
  return _pAzimutInRadian;
}

double SeismPolarizationAnalysisData::getIncidenceInRadian() const {
  return _pIncidenceInRadian;
}

double SeismPolarizationAnalysisData::getAzimutDegrees() const {
  return _pAzimutDegrees;
}

double SeismPolarizationAnalysisData::getIncidenceDegrees() const {
  return _pIncidenceDegrees;
}

} // namespace Data
