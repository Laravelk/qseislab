#pragma once

#include <memory>

#include <QJsonObject>
#include <QObject>

namespace Data {
class SeismTrace : public QObject {
  Q_OBJECT
public:
  explicit SeismTrace();

  explicit SeismTrace(const SeismTrace &);

  //  void triggerChange() const;

  float getMaxValue() const;

  int getBufferSize() const;

  const std::unique_ptr<float[]> &getBuffer() const;
  void setBuffer(uint32_t, float *);

  QJsonObject &writeToJson(QJsonObject &) const;

signals:
  void changed() const;

private:
  float _maxValue{-1.0};

  //    int _dataFormat; // NOTE: use later

  uint32_t _bufferSize{0};

  // NOTE:
  //  может просто хранить указатель, без умного укащателя?
  std::unique_ptr<float[]> _buffer;
};

} // namespace Data
