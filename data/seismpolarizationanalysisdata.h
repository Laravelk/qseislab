#pragma once

namespace Data {
class SeismPolarizationAnalysisData {
public:
  explicit SeismPolarizationAnalysisData(double maxSingularValue, double pAzimutInRadian,
                                double pIncidenceInRadian,
                                double pAzimutDegrees,
                                double pIncidenceDegrees);

  void setValid(const bool validStatus);
  void setMaxSingularVale(const double value);
  void setAzimutInRadian(const double radian);
  void setIncidenceInRadian(const double radian);

  bool isValid() const;
  double getMaxSingularValue() const;
  double getAzimutInRadian() const;
  double getIncidenceInRadian() const;
  double getAzimutDegrees() const;
  double getIncidenceDegrees() const;

  void print();

private:
  bool _isValid;
  double _maxSingularValue;
  double _pAzimutInRadian;
  double _pIncidenceInRadian;

  const double DEGREES_COEFFICIENT = 180;
};
} // namespace Data
