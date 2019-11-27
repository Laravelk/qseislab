#include "share_functions.h"

#include "data/seismcomponent.h"

namespace EventOperation {
namespace Modefication {
bool rotateDataWithTransitionMatrix(Data::SeismComponent *const component,
                                    const Eigen::MatrixXf &transitionMatrix) {
  const int size = component->getTracesAmount();
  if (size != transitionMatrix.cols() && size != transitionMatrix.rows()) {
    return false;
  }

  int trace_size = component->getTraceSize();

  Eigen::MatrixXf b_data(size, trace_size);
  int row = 0;
  for (auto &trace : component->getTraces()) {
    Eigen::Map<Eigen::RowVectorXf> b_data_vec(trace->getBuffer(), trace_size);
    b_data.row(row) = b_data_vec;
    ++row;
  }

  auto e_data = transitionMatrix * b_data;
  row = 0;
  for (auto &trace : component->getTraces()) {
    float *buffer = trace->getBuffer();
    for (int col = 0; col < trace->getBufferSize(); ++col) {
      buffer[col] = e_data(row, col);
    }
    ++row;
  }

  return true;
}

} // namespace Modefication
} // namespace EventOperation
