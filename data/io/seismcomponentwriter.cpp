#include "seismcomponentwriter.h"

namespace Data {
namespace IO {
SeismComponentWriter::SeismComponentWriter(const QFileInfo &fileInfo)
    : _file(fileInfo.absoluteFilePath()), _outstream(&_file) {
  if (!_file.open(QIODevice::WriteOnly)) {
    throw std::runtime_error("File can not be opened (SeismComponentWriter)");
  }

  _outstream.setFloatingPointPrecision(QDataStream::SinglePrecision);
}

void SeismComponentWriter::writeComponent(
    SeismComponent const *const component) {
  //  const std::vector<std::shared_ptr<SeismTrace>> &traces =
  //      component->getTraces();
  //  for (unsigned i = 0; i < traces.size(); ++i) {
  //    writeTrace(traces[i]);
  //  }

  for (auto trace : component->getTraces()) {
    writeTrace(trace);
  }
}

SeismComponentWriter::~SeismComponentWriter() { _file.close(); }

void SeismComponentWriter::writeTrace(SeismTrace const *const trace) {
  int bufferSize = trace->getBufferSize();
  //  const std::unique_ptr<float[]> &buffer = trace->getBuffer();
  auto buffer = trace->getBuffer();

  _outstream << bufferSize;
  for (int i = 0; i < bufferSize; ++i) {
    _outstream << buffer[static_cast<unsigned>(i)];
  }
}

} // namespace IO
} // namespace Data
