#pragma once

#include "data/seismpolarizationanalysisdata.h"
#include "data/seismwavepick.h"
#include "undo_stack_work/eventoperationundocommand.h"

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

class PolarizationAnalysisCompute : public EventOperationUndoCommand {
public:

    class Parameters {
    public:
        explicit Parameters() {}
    private:
    };

    explicit PolarizationAnalysisCompute(const std::set<Data::SeismEvent *> &, const Parameters &);

protected:
  void undoForOne(Data::SeismEvent *) override;
  void redoForOne(Data::SeismEvent *) override;

private:

  void calculate();

  Data::SeismEvent *_event;
  const double DEGREES_COEFFICIENT = 180;

  std::map<std::pair<int, Data::SeismWavePick::Type>,
           std::optional<Data::SeismPolarizationAnalysisData>>
      _oldDataMap;

private:
  Eigen::MatrixXf getPointMatrix(Data::SeismComponent *const,
                                  const int firstIndex, const int lastIndex);
  Data::SeismPolarizationAnalysisData
  calculatePolarizationData(const Eigen::MatrixXf &);

  template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }
};
