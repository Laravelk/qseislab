#include "seismpolarizationanalysisdata.h"

#include <QtMath>
#include <iostream> // TODO: delete

namespace Data {

SeismPolarizationAnalysisData::SeismPolarizationAnalysisData(
    double maxSingularValue, double pAzimutInRadian, double pIncidenceInRadian,
    double pAzimutDegrees, double pIncidenceDegrees, float planarity,
    float recilinearity, QVector3D eigenVector)
    : _maxSingularValue(maxSingularValue), _pAzimutInRadian(pAzimutInRadian),
      _pIncidenceInRadian(pIncidenceInRadian), _eigenVector(eigenVector),
      _planarity(planarity), _rectilinearity(recilinearity) {}

void SeismPolarizationAnalysisData::setValid(const bool validStatus) {
  _isValid = validStatus;
}

void SeismPolarizationAnalysisData::setMaxSingularVale(const double value) {
  _maxSingularValue = value;
}

void SeismPolarizationAnalysisData::setAzimutInRadian(const double radian) {
  _pAzimutInRadian = radian;
}

void SeismPolarizationAnalysisData::setIncidenceInRadian(const double radian) {
  _pIncidenceInRadian = radian;
}

SeismPolarizationAnalysisData &SeismPolarizationAnalysisData::
operator=(const SeismPolarizationAnalysisData &pick) {
  if (this != &pick) {
    this->_pAzimutInRadian = pick._pAzimutInRadian;
    this->_pIncidenceInRadian = pick._pIncidenceInRadian;
    this->_isValid = pick._isValid;
    this->_maxSingularValue = pick._maxSingularValue;
    this->_eigenVector = pick._eigenVector;
    this->_planarity = pick._planarity;
    this->_rectilinearity = pick._rectilinearity;
  }
  return *this;
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
  return _pAzimutInRadian * DEGREES_COEFFICIENT / M_PI;
}

double SeismPolarizationAnalysisData::getIncidenceDegrees() const {
  return _pIncidenceInRadian * DEGREES_COEFFICIENT / M_PI;
}

void SeismPolarizationAnalysisData::print() {
  std::cerr << "maxSing " << _maxSingularValue << " AzimputInRadian "
            << _pAzimutInRadian << " IncendceInRadian " << _pIncidenceInRadian
            << "planarity " << _planarity
            << " rectilinearity: " << _rectilinearity << std::endl;
}

QVector3D SeismPolarizationAnalysisData::getEigenVector() const {
  return _eigenVector;
}

void SeismPolarizationAnalysisData::setEigenVector(const QVector3D &value) {
  _eigenVector = value;
}

float SeismPolarizationAnalysisData::getRectilinearity() const {
  return _rectilinearity;
}

void SeismPolarizationAnalysisData::setRectilinearity(float rectilinearity) {
  _rectilinearity = rectilinearity;
}

float SeismPolarizationAnalysisData::getPlanarity() const { return _planarity; }

void SeismPolarizationAnalysisData::setPlanarity(float planarity) {
  _planarity = planarity;
}

} // namespace Data
