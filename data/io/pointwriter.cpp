#include "pointwriter.h"

namespace Data {
namespace IO {
PointWriter::PointWriter(const QFileInfo &fileInfo)
    : _file(fileInfo.absoluteFilePath()), _outstream(&_file) {
  if (!_file.open(QIODevice::WriteOnly)) {
    throw std::runtime_error("File can not be opened (SeismPointWriter)");
  }

  _outstream.setFloatingPointPrecision(QDataStream::SinglePrecision);
  _outstream.setByteOrder(QDataStream::LittleEndian);
}

void PointWriter::writePoint(const Point &point) {
  _outstream << std::get<0>(point) << std::get<1>(point) << std::get<2>(point);
}

PointWriter::~PointWriter() { _file.close(); }

} // namespace IO
} // namespace Data
