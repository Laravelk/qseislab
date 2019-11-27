#pragma once

#include <Eigen/Dense>

#include <memory>

namespace Data {
class SeismComponent;
}

namespace EventOperation {
namespace Modefication {
bool rotateDataWithTransitionMatrix(Data::SeismComponent *const,
                                    const Eigen::MatrixXf &);
}
} // namespace EventOperation
