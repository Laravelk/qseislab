#include "seismcomponentreader.h"

#include "data/seismtrace.h"

namespace Data {
namespace IO {
SeismComponentReader::SeismComponentReader(const QFileInfo &fileInfo)
    : _file(fileInfo.absoluteFilePath()), _instream(&_file) {
  if (!_file.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("File can not be opened");
  }

  _instream.setFloatingPointPrecision(QDataStream::SinglePrecision);
}

bool SeismComponentReader::hasNext() const { return !_instream.atEnd(); }

std::unique_ptr<SeismTrace> SeismComponentReader::nextTrace() {
  auto trace = std::make_unique<Data::SeismTrace>();
  uint32_t buf_size;
  _instream >> buf_size;
  float *buffer = new float[buf_size];
  for (uint32_t j = 0; j < buf_size; ++j) {
    _instream >> buffer[j];
  }
  trace->setBuffer(buf_size, buffer);
  delete[] buffer;
  return trace;
}

// std::vector<std::pair<uint32_t, std::unique_ptr<float[]>>>&
// SeismComponentReader::nextData()
//{
//    _data.clear();

//    for(int i = 0; i < TRACE_IN_COMPONENT; ++i) {
//        uint32_t buf_size;
//        _instream >> buf_size;
//        auto buffer = std::make_unique<float[]>(buf_size);
//        for (uint32_t j = 0; j < buf_size; ++j) {
//            _instream >> buffer[j];
//        }

//        _data.push_back( {buf_size, std::move(buffer)} );
//    }

//    return _data;
//}

SeismComponentReader::~SeismComponentReader() { _file.close(); }

} // namespace IO
} // namespace Data
