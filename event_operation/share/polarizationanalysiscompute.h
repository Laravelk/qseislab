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
class PolarizationAnalysisData : QUndoCommand {
public:
  PolarizationAnalysisData(std::unique_ptr<Data::SeismEvent> &);
  bool calculatePolarizationData();

  void undo() override;
  void redo() override;

private:
  Data::SeismEvent *_event;

private:
  Eigen::MatrixXf *getPointMatrix(const std::unique_ptr<Data::SeismComponent> &,
                                  const int firstIndex, const int lastIndex);
  void calculateSVD(const Eigen::MatrixXf &);

  template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }
};
} // namespace EventOperation
