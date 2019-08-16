#include "seismpointreader.h"

namespace Data {
namespace IO {
SeismPointReader::SeismPointReader(const QFileInfo &fileInfo)
    : _file(fileInfo.absoluteFilePath()), _instream(&_file) {
  if (!_file.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("File can not be opened (SeismPointReader)");
  }

  _instream.setByteOrder(QDataStream::LittleEndian);
  _instream.setFloatingPointPrecision(QDataStream::SinglePrecision);
}

bool SeismPointReader::hasNext() const { return !_instream.atEnd(); }

SeismHorizon::SeismPoint SeismPointReader::next() {
  float x;
  float y;
  float z;

  _instream >> x >> y >> z;

  return SeismHorizon::SeismPoint(x, y, z);
}

SeismPointReader::~SeismPointReader() { _file.close(); }

} // namespace IO
} // namespace Data
