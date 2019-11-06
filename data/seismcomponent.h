#pragma once

#include "seism_data_type.h"
#include "seismtrace.h"
#include "seismwavepick.h"

#include <QDateTime>
#include <QJsonObject>
#include <QObject>
#include <QUuid>

#include <map>
#include <memory>
#include <vector>

namespace Data {
class SeismComponent : public QObject {
  Q_OBJECT
public:
  explicit SeismComponent(const QUuid &);

  explicit SeismComponent(const QJsonObject &json) noexcept(false);

  explicit SeismComponent(const SeismComponent &);

  const QUuid &getReceiverUuid() const;

  const QDateTime &getStampDateTime() const;
  void setStampDateTime(const QDateTime &);

  float getSampleInterval() const;
  void setSampleInterval(float);

  float getMaxValue() const;

  void addTrace(std::unique_ptr<SeismTrace>);

  unsigned getTracesAmount() const;
  const std::vector<std::unique_ptr<SeismTrace>> &getTraces() const;

  void addWavePick(SeismWavePick);
  void removeWavePick(const SeismWavePick::Type);
  bool containsWavePickBy(const SeismWavePick::Type) const;
  const SeismWavePick &getWavePick(SeismWavePick::Type) const;
  const std::map<SeismWavePick::Type, SeismWavePick> &getWavePicks() const;

  QJsonObject &writeToJson(QJsonObject &) const;

signals:
  void changed();

private:
  QUuid _receiverUuid;
  QDateTime _stampDateTime;
  float _sampleInterval{0.0};
  float _maxValue{-1.0};
  std::vector<std::unique_ptr<SeismTrace>> _traces;
  std::map<SeismWavePick::Type, SeismWavePick> _wavePicks_map;
};

} // namespace Data
