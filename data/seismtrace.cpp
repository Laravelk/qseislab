#include "seismtrace.h"

#include <assert.h>

namespace Data {
SeismTrace::SeismTrace() {}

SeismTrace::SeismTrace(const SeismTrace &other)
    : _maxValue(other._maxValue), _bufferSize(other._bufferSize),
      _buffer(std::make_unique<float[]>(_bufferSize)) {
  for (uint32_t i = 0; i < _bufferSize; ++i) {
    _buffer[i] = other._buffer[i];
  }
}

float SeismTrace::getMaxValue() const { return _maxValue; }

int SeismTrace::getBufferSize() const { return static_cast<int>(_bufferSize); }

const std::unique_ptr<float[]> &SeismTrace::getBuffer() const {
  return _buffer;
}

void SeismTrace::setBuffer(uint32_t size, float *buffer) {
  assert(0 <= size);

  _bufferSize = size;

  _buffer.reset();
  _buffer = std::make_unique<float[]>(_bufferSize);
  for (uint32_t i = 0; i < _bufferSize; ++i) {
    _buffer[i] = buffer[i];
    if (_maxValue < buffer[i]) {
      _maxValue = buffer[i];
    }
  }

  emit changed();
}

QJsonObject &SeismTrace::writeToJson(QJsonObject &json) const {
  json["sampleAmount"] = static_cast<int>(_bufferSize);
  return json;
}

} // namespace Data
