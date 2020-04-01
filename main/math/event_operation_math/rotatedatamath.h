#pragma once

#include <Eigen/Dense>

namespace Data {
class SeismComponent;
}

namespace EventMath {
class RotateDataMath
{
public:
    static bool rotateDataWithTransitionMatrix( Data::SeismComponent *const, const Eigen::Matrix3f &);
private:
    RotateDataMath(){}
};
}

