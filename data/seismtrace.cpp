#include "seismtrace.h"

namespace Data {
SeismTrace::SeismTrace() {}

SeismTrace::SeismTrace(const QJsonObject &json,
                       std::pair<uint32_t, std::unique_ptr<float[]>> &data) {
  std::string err_msg;
  if (json.contains("sampleInterval")) {
    _sampleInterval = static_cast<float>(json["sampleInterval"].toDouble());
  } else {
    err_msg += "::sampleInterval : not found\n";
  }

  _bufferSize = data.first;
  _buffer = std::move(data.second);

  if (json.contains("sampleNumber")) {
    int jsize = json["sampleNumber"].toInt();
    if (jsize != static_cast<int>(_bufferSize)) {
      err_msg += "::data : data-size in json-file doesn`t match data-size in "
                 "bin-file\n";
    }
  } else {
    err_msg += "::sampleNumber : not found\n";
  }

  if (!err_msg.empty()) {
    err_msg += "\n";
    throw std::runtime_error(err_msg);
  }

  // calc _maxValue
  for (uint32_t i = 0; i < _bufferSize; ++i) {
    if (_buffer[i] > _maxValue) {
      _maxValue = _buffer[i];
    }
  }
}

float SeismTrace::getSampleInterval() const { return _sampleInterval; }

void SeismTrace::setSampleInterval(float sampleInterval) {
  _sampleInterval = sampleInterval;
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
}

QJsonObject &SeismTrace::writeToJson(QJsonObject &json) const {
  json["sampleInterval"] = static_cast<double>(_sampleInterval);
  json["sampleNumber"] = static_cast<int>(_bufferSize);
  return json;
}

} // namespace Data
