#include "horizonreadergrd.h"

namespace Data {
namespace IO {
HorizonReaderGRD::HorizonReaderGRD(const QFileInfo &fileInfo)
    : _file(fileInfo.absoluteFilePath()), _in(&_file) {

  if (!_file.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("File can not be opened (HorizonReaderGRD)");
  }
  _in.setByteOrder(QDataStream::LittleEndian);
  _in.setFloatingPointPrecision(QDataStream::SinglePrecision);

  _in >> _x_cur >> _x_step >> _x_max;
  _in >> _y_cur >> _y_step >> _y_max;

  _Nx = static_cast<int>((_x_max - _x_cur) / _x_step) + 1;
  _Ny = static_cast<int>((_y_max - _y_cur) / _y_step) + 1;
}

bool HorizonReaderGRD::hasNext() const {
  return !_in.atEnd() && (_x_cur <= _x_max) && (_y_cur <= _y_max);
}

Point HorizonReaderGRD::next() {
  float z;
  _in >> z;

  auto point = Point(_x_cur, _y_cur, z);

  _x_cur += _x_step;
  _y_cur += _y_step;

  return point;
}

int HorizonReaderGRD::getNx() const { return _Nx; }

int HorizonReaderGRD::getNy() const { return _Ny; }

HorizonReaderGRD::~HorizonReaderGRD() { _file.close(); }

} // namespace IO
} // namespace Data
