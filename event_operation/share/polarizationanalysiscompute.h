#pragma once

#include "data/seismwavepick.h"
#include "data/seismpolarizationanalysisdata.h"

#include <QUndoCommand>
#include <Eigen/Eigenvalues>
#include <Eigen/Jacobi>
#include <Eigen/SVD>
#include <map>
#include <memory>

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
class SeismWavePick;
} // namespace Data

namespace EventOperation {
class PolarizationAnalysisCompute : QUndoCommand {
public:
  PolarizationAnalysisCompute(std::unique_ptr<Data::SeismEvent> &);
  void calculate();

  void undo() override;
  void redo() override;

private:
  Data::SeismEvent *_event;
  const double DEGREES_COEFFICIENT = 180;

  std::map<std::pair<int, Data::SeismWavePick::Type>,
    std::optional<Data::SeismPolarizationAnalysisData*>> _oldDataMap;

private:
  Eigen::MatrixXf *getPointMatrix(const std::unique_ptr<Data::SeismComponent> &,
                                  const int firstIndex, const int lastIndex);
  Data::SeismPolarizationAnalysisData *calculatePolarizationData(const Eigen::MatrixXf &);

  template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }
};
} // namespace EventOperation
