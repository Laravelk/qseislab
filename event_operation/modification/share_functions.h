#pragma once

#include <Eigen/Dense>

#include <memory>

namespace Data {
class SeismComponent;
}

bool rotateDataWithTransitionMatrix(Data::SeismComponent *const,
                                    const Eigen::Matrix3f &);
