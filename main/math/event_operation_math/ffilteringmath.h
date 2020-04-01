#pragma once

#include <vector>
#include <unsupported/Eigen/FFT>
#include <math.h>

namespace EventMath {
class FFilteringMath {
public:
  static void FFiltering(std::vector<float> &, const int, const int, const int,
                         const int, const float);

private:
  FFilteringMath() {}
  constexpr static const int MICROSECONDS_IN_SECONDS = 1000000;
};
}
