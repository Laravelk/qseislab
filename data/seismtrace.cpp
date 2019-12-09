#include "seismtrace.h"

#include <assert.h>

namespace Data {
SeismTrace::SeismTrace() {}

SeismTrace::SeismTrace(const SeismTrace &other)
    : _maxValue(other._maxValue), _bufferSize(other._bufferSize),
      _orientation(other._orientation),
      _buffer(std::make_unique<float[]>(_bufferSize)) {
  for (uint32_t i = 0; i < _bufferSize; ++i) {
    _buffer[i] = other._buffer[i];
  }
}

float SeismTrace::getMaxValue() const { return _maxValue; }

int SeismTrace::getBufferSize() const { return static_cast<int>(_bufferSize); }

const Point &SeismTrace::getOrientation() const { return _orientation; }

void SeismTrace::setOrientation(const Point &orientation) {
  _orientation = orientation;
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

const float *SeismTrace::getBuffer() const { return _buffer.get(); }

float *SeismTrace::getBuffer() { return _buffer.get(); }

QJsonObject &SeismTrace::writeToJson(QJsonObject &json) const {
  json["sampleAmount"] = static_cast<int>(_bufferSize);
  return json;
}

} // namespace Data
