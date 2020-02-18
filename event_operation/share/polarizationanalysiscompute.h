#pragma once

#include "data/seismpolarizationanalysisdata.h"
#include "data/seismwavepick.h"

#include <Eigen/Eigenvalues>
#include <Eigen/Jacobi>
#include <Eigen/SVD>
#include <QUndoCommand>
#include <map>
#include <memory>

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
class SeismWavePick;
} // namespace Data

namespace EventOperation {
class PolarizationAnalysisCompute /*: public QUndoCommand*/ {
public:
  PolarizationAnalysisCompute(Data::SeismEvent * const);
  void calculate();

  //  void undo() override;
  //  void redo() override;

private:
  Data::SeismEvent *_event;
  const double DEGREES_COEFFICIENT = 180;

  std::map<std::pair<int, Data::SeismWavePick::Type>,
           std::optional<Data::SeismPolarizationAnalysisData>>
      _oldDataMap;

  std::map<std::pair<int, Data::SeismWavePick::Type>,
           std::optional<Data::SeismPolarizationAnalysisData>>
      _currentlyMap;

private:
  Eigen::MatrixXf getPointMatrix(Data::SeismComponent *const,
                                  const int firstIndex, const int lastIndex);
  Data::SeismPolarizationAnalysisData
  calculatePolarizationData(const Eigen::MatrixXf &);

  int test_data_for_print_num_of_comp = 0;
  Data::SeismWavePick::Type test_data_for_print_type;
  template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }
};
} // namespace EventOperation
