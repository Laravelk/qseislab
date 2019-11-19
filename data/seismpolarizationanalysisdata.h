#pragma once

namespace Data {
class SeismPolarizationAnalysisData {
public:
  SeismPolarizationAnalysisData(double maxSingularValue, double pAzimutInRadian,
                                double pIncidenceInRadian,
                                double pAzimutDegrees,
                                double pIncidenceDegrees);

  void setValid(const bool validStatus);
  void setMaxSingularVale(const double value);
  void setAzimutInRadian(const double radian);
  void setIncidenceInRadian(const double radian);
  void setAzimutDegrees(const double degrees);
  void setIncidenceDegrees(const double degrees);

  bool isValid() const;
  double getMaxSingularValue() const;
  double getAzimutInRadian() const;
  double getIncidenceInRadian() const;
  double getAzimutDegrees() const;
  double getIncidenceDegrees() const;

private:
  bool _isValid = false;
  double _maxSingularValue = 0;
  double _pAzimutInRadian = 0;
  double _pIncidenceInRadian = 0;
  double _pAzimutDegrees = 0;
  double _pIncidenceDegrees = 0;

  const double DEGREES_COEFFICIENT = 180;
};
} // namespace Data
