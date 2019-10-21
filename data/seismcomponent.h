#pragma once

#include "seism_data_type.h"
#include "seismtrace.h"
#include "seismwavepick.h"

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

  float getSampleInterval() const;
  void setSampleInterval(float);

  float getMaxValue() const;

  void addTrace(std::unique_ptr<SeismTrace>);

  unsigned getTracesNumber() const;
  const std::vector<std::unique_ptr<SeismTrace>> &getTraces() const;

  void addWavePick(const SeismWavePick &);
  void removeWavePick(SeismWavePick::Type);
  const SeismWavePick &getWavePick(SeismWavePick::Type) const;
  const std::map<SeismWavePick::Type, SeismWavePick> &getWavePicks() const;

  QJsonObject &writeToJson(QJsonObject &) const;

signals:
  void changed();

private:
  QUuid _receiverUuid;
  float _sampleInterval{0.0};
  float _maxValue{-1.0};
  std::vector<std::unique_ptr<SeismTrace>> _traces;
  std::map<SeismWavePick::Type, SeismWavePick> _wavePicks_map;
};

} // namespace Data
