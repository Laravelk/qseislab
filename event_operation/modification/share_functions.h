#pragma once

#include <Eigen/Dense>

#include <memory>

namespace Data {
class SeismComponent;
}

namespace EventOperation {
namespace Modefication {
bool rotateDataWithTransitionMatrix(const std::unique_ptr<Data::SeismComponent>&, const Eigen::MatrixXf& );
}
}

