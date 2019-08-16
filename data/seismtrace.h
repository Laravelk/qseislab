#pragma once

#include <memory>

#include <QJsonObject>

namespace Data {
class SeismTrace {
public:
  explicit SeismTrace();
  explicit SeismTrace(
      const QJsonObject &,
      std::pair<uint_fast32_t, std::unique_ptr<float[]>> &) noexcept(false);

  float getSampleInterval() const;
  void setSampleInterval(float);

  float getMaxValue() const;

  int getBufferSize() const;

  const std::unique_ptr<float[]> &getBuffer() const;
  void setBuffer(uint32_t, float *);

  QJsonObject &writeToJson(QJsonObject &) const;

private:
  float _sampleInterval{0.0};

  float _maxValue{-1.0};

  //    int _dataFormat; // NOTE: use later

  uint32_t _bufferSize{0};
  std::unique_ptr<float[]> _buffer;
};

} // namespace Data
