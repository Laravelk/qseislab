#pragma once

#include "seism_data_type.h"

#include <memory>

#include <QJsonObject>
#include <QObject>

namespace Data {
class SeismTrace : public QObject {
  Q_OBJECT
public:
  explicit SeismTrace();

  explicit SeismTrace(const SeismTrace &);

  float getMaxValue() const;

  int getBufferSize() const;

  const Point &getOrientation() const;
  void setOrientation(const Point &);

  void setBuffer(uint32_t, float *);
  float const *getBuffer() const;
  float *getBuffer();

  QJsonObject &writeToJson(QJsonObject &) const;

signals:
  void changed() const;

private:
  float _maxValue{-1.0};

  //    int _dataFormat; // NOTE: use later

  uint32_t _bufferSize{0};

  Point _orientation;

  // NOTE:
  //  может просто хранить указатель, без умного укащателя?
  std::unique_ptr<float[]> _buffer;
};

} // namespace Data
