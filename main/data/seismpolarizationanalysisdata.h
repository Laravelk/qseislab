#pragma once

#include <QVector3D>

namespace Data {
class SeismPolarizationAnalysisData {
public:
  explicit SeismPolarizationAnalysisData(double maxSingularValue, double pAzimutInRadian,
                                double pIncidenceInRadian,
                                double pAzimutDegrees,
                                double pIncidenceDegrees, float planarity, float rectilinearity, QVector3D eigenVector);

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

  SeismPolarizationAnalysisData &operator=(SeismPolarizationAnalysisData const &pick);

  void print();

  QVector3D getEigenVector() const;
  void setEigenVector(const QVector3D &value);

  float getRectilinearity() const;
  void setRectilinearity(float rectilinearity);

  float getPlanarity() const;
  void setPlanarity(float planarity);

private:
  bool _isValid;
  double _maxSingularValue;
  double _pAzimutInRadian;
  double _pIncidenceInRadian;
  float _planarity;
  float _rectilinearity;
  QVector3D _eigenVector;

  const double DEGREES_COEFFICIENT = 180;
};
} // namespace Data
