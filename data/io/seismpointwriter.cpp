#include "seismpointwriter.h"

namespace Data {
namespace IO {
SeismPointWriter::SeismPointWriter(const QFileInfo &fileInfo, int pointNum)
    : _file(fileInfo.absoluteFilePath()), _outstream(&_file) {
  if (!_file.open(QIODevice::WriteOnly)) {
    throw std::runtime_error("File can not be opened");
  }

  _outstream << pointNum;
}

void SeismPointWriter::writePoint(const SeismHorizon::SeismPoint &point) {
  _outstream << std::get<0>(point) << std::get<1>(point) << std::get<2>(point)
             << std::get<3>(point);
}

SeismPointWriter::~SeismPointWriter() { _file.close(); }

} // namespace IO
} // namespace Data
