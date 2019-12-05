#include "seismpolarizationanalysisdata.h"

#include <QtMath>
#include <iostream> // TODO: delete

namespace Data {

SeismPolarizationAnalysisData::SeismPolarizationAnalysisData(double maxSingularValue, double pAzimutInRadian,
                                                             double pIncidenceInRadian, double pAzimutDegrees,
                                                             double pIncidenceDegrees)
    : _maxSingularValue(maxSingularValue), _pAzimutInRadian(pAzimutInRadian), _pIncidenceInRadian(pIncidenceInRadian) {}

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

SeismPolarizationAnalysisData &SeismPolarizationAnalysisData::operator=(const SeismPolarizationAnalysisData &pick)
{
    if (this != &pick) {
        this->_pAzimutInRadian = pick._pAzimutInRadian;
        this->_pIncidenceInRadian = pick._pIncidenceInRadian;
        this->_isValid = pick._isValid;
        this->_maxSingularValue = pick._maxSingularValue;
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

void SeismPolarizationAnalysisData::print()
{
    std::cerr << "maxSing " << _maxSingularValue << " AzimputInRadian " << _pAzimutInRadian << " IncendceInRadian " <<
                 _pIncidenceInRadian << std::endl;

}

} // namespace Data
