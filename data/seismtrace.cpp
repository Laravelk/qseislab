#include "seismtrace.h"

namespace Data {
SeismTrace::SeismTrace() {}

SeismTrace::SeismTrace(const QJsonObject &json, std::unique_ptr<float[]> data) {
  if (!(json.contains("sampleInterval") && json.contains("pWaveArrival") &&
        json.contains("sWaveArrival") && json.contains("maxValue"))) {
    throw std::runtime_error("Not found json-field (SeismTrace)");
  }

  _sampleInterval = static_cast<float>(json["sampleInterval"].toDouble());
  int size = json["size"].toInt();

  if (0 > size) {
    throw std::runtime_error("Invalid values for json-field");
  }

  _bufferSize = static_cast<unsigned>(size);

  _pWaveArrival = static_cast<float>(json["pWaveArrival"].toDouble());
  _sWaveArrival = static_cast<float>(json["sWaveArrival"].toDouble());

  _maxValue = static_cast<float>(json["maxValue"].toDouble());

  _buffer = std::move(data);
}

float SeismTrace::getSampleInterval() const { return _sampleInterval; }

void SeismTrace::setSampleInterval(float sampleInterval) {
  _sampleInterval = sampleInterval;
}

float SeismTrace::getPWaveArrival() const { return _pWaveArrival; }

void SeismTrace::setPWaveArrival(float pWaveArrival) {
  _pWaveArrival = pWaveArrival;
}

float SeismTrace::getSWaveArrival() const { return _sWaveArrival; }

void SeismTrace::setSWaveArrival(float sWaveArrival) {
  _sWaveArrival = sWaveArrival;
}

float SeismTrace::getMaxValue() const { return _maxValue; }

unsigned SeismTrace::getBufferSize() const { return _bufferSize; }

const std::unique_ptr<float[]> &SeismTrace::getBuffer() const {
  return _buffer;
}

void SeismTrace::setBuffer(unsigned size, float *buffer) {
  assert(0 <= size);

  _bufferSize = size;

  _buffer.reset();
  _buffer = std::make_unique<float[]>(_bufferSize);
  for (unsigned i = 0; i < _bufferSize; ++i) {
    _buffer[i] = buffer[i];
    if (_maxValue < buffer[i]) {
      _maxValue = buffer[i];
    }
  }
}

QJsonObject &SeismTrace::writeToJson(QJsonObject &json) const {
  json["sampleInterval"] = static_cast<double>(_sampleInterval);
  json["pWaveArrival"] = static_cast<double>(_pWaveArrival);
  json["sWaveArrival"] = static_cast<double>(_sWaveArrival);
  json["maxValue"] = static_cast<double>(_maxValue);
  json["size"] = static_cast<int>(_bufferSize);
  return json;
}

} // namespace Data
