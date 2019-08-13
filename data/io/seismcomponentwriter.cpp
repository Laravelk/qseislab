#include "seismcomponentwriter.h"

namespace Data {
namespace IO {
SeismComponentWriter::SeismComponentWriter(const QFileInfo &fileInfo,
                                           int componentNum,
                                           int tracesInComponent)
    : _file(fileInfo.absoluteFilePath()), _outstream(&_file) {
  if (!_file.open(QIODevice::WriteOnly)) {
    throw std::runtime_error("File can not be opened (SeismComponentWriter)");
  }

  _outstream << componentNum << tracesInComponent;
}

void SeismComponentWriter::writeComponent(
    const std::unique_ptr<SeismComponent> &component) {
  const std::vector<std::unique_ptr<SeismTrace>> &traces =
      component->getTraces();
  for (unsigned i = 0; i < component->getTracesNumber(); ++i) {
    writeTrace(traces[i]);
  }
}

SeismComponentWriter::~SeismComponentWriter() { _file.close(); }

void SeismComponentWriter::writeTrace(
    const std::unique_ptr<SeismTrace> &trace) {
  unsigned bufferSize = trace->getBufferSize();
  const std::unique_ptr<float[]> &buffer = trace->getBuffer();

  _outstream << bufferSize;
  for (unsigned i = 0; i < bufferSize; ++i) {
    _outstream << buffer[i];
  }
}

} // namespace IO
} // namespace Data
