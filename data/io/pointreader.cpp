#include "pointreader.h"

namespace Data {
namespace IO {
PointReader::PointReader(const QFileInfo &fileInfo)
    : _file(fileInfo.absoluteFilePath()), _instream(&_file) {
  if (!_file.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("File can not be opened (SeismPointReader)");
  }

  _instream.setByteOrder(QDataStream::LittleEndian);
  _instream.setFloatingPointPrecision(QDataStream::SinglePrecision);
}

bool PointReader::hasNext() const { return !_instream.atEnd(); }

Point PointReader::next() {
  float x;
  float y;
  float z;

  _instream >> x >> y >> z;

  return Point(x, y, z);
}

PointReader::~PointReader() { _file.close(); }

} // namespace IO
} // namespace Data
