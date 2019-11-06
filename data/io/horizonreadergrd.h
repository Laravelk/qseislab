#pragma once

#include "data/seism_data_type.h"

#include <QDataStream>
#include <QFileInfo>

namespace Data {
namespace IO {
class HorizonReaderGRD {
public:
  explicit HorizonReaderGRD(const QFileInfo &) noexcept(false);

  bool hasNext() const;

  Point next();

  int getNx() const;
  int getNy() const;

  ~HorizonReaderGRD();

private:
  QFile _file;
  QDataStream _in;

  float _x_cur;
  float _x_step;
  float _x_max;

  float _y_cur;
  float _y_step;
  float _y_max;

  int _Nx;
  int _Ny;
};
} // namespace IO
} // namespace Data
