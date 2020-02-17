#include "pointreader.h"

namespace Data {
namespace IO {
PointReader::PointReader(const QFileInfo &fileInfo)
    : _file(fileInfo.absoluteFilePath()), _instreamText(&_file),
      _instreamBin(&_file) {

  if ("bin" == fileInfo.suffix()) {
    _isBin = true;
    if (!_file.open(QIODevice::ReadOnly)) {
      throw std::runtime_error("File can not be opened (SeismPointReader)");
    }
    _instreamBin.setByteOrder(QDataStream::LittleEndian);
    _instreamBin.setFloatingPointPrecision(QDataStream::SinglePrecision);
  } else {
    _isBin = false;
    if (!_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      throw std::runtime_error("File can not be opened (SeismPointReader)");
    }
  }
}

bool PointReader::hasNext() const {
  if (_isBin)
    return !_instreamBin.atEnd();
  else
    return !_instreamText.atEnd();
}

Point PointReader::next() {
  float x;
  float y;
  float z;

  if (_isBin)
    _instreamBin >> x >> y >> z;
  else
    _instreamText >> z >> x >> y; // NOTE: в файле потому что так

  return Point(x, y, z);
}

PointReader::~PointReader() { _file.close(); }

} // namespace IO
} // namespace Data
