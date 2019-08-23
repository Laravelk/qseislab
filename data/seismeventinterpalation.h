#pragma once

#include "seism_data_type.h"
#include <boost/math/interpolators/cubic_b_spline.hpp>
#include <vector>

namespace Data {
class SeismEventInterpalation {
public:
  SeismEventInterpalation();
  SeismEventInterpalation(std::vector<Point> getPoints);

  const std::vector<Point> getPoints() const;
  int getNx() const;
  int getNy() const;

  void setPoints(std::vector<Point> pointVector);
  void setNx(int Nx);
  void setNy(int Ny);

private:
  std::vector<Point> _pointVector;
  int _Nx;
  int _Ny;

private:
};
} // namespace Data
