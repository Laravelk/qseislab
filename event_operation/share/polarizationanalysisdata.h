#pragma once

#include "data/seismwavepick.h"

#include <QUndoCommand>
#include <eigen3/Eigen/Eigenvalues>
#include <eigen3/Eigen/Jacobi>
#include <eigen3/Eigen/SVD>
#include <memory>

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
class SeismWavePick;
} // namespace Data

namespace EventOperation {
class PolarizationAnalysisData : QUndoComand {
public:
  PolarizationAnalysisData(std::unique_ptr<Data::SeismEvent> &);
  bool calculatePolarizationData();

private:
  bool _isValid = false;
  Data::SeismEvent *_event;
  Data::SeismWavePick::Type _type;
  int _numberComponent;

  double _maxSingularValue = 0;
  double _pAzimutInRadian = 0;
  double _pIncidenceInRadian = 0;

  double _pAzimutDegrees = 0;
  double _pIncidenceDegrees = 0;

private:
  Eigen::MatrixXf *getPointMatrix(const std::unique_ptr<Data::SeismComponent> &,
                                  const int firstIndex, const int lastIndex);
  void calculateSVD(const Eigen::MatrixXf &);

  template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }
};
} // namespace EventOperation
