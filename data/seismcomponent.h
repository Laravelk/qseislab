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
  //  explicit SeismComponent(
  //      const QJsonObject &json,
  //      std::vector<std::pair<uint32_t, std::unique_ptr<float[]>>>
  //          &data) noexcept(false);

  explicit SeismComponent(const QJsonObject &json) noexcept(false);

  explicit SeismComponent(const SeismComponent &);

  //  const QUuid &getWellUuid() const;
  const QUuid &getReceiverUuid() const;

  float getSampleInterval() const;
  void setSampleInterval(float);

  //  int getPWaveArrival() const;
  //  void setPWaveArrival(int);

  //  int getSWaveArrival() const;
  //  void setSWaveArrival(int);

  float getMaxValue() const;

  //  const std::unique_ptr<SeismReceiver> &getReceiver() const;

  //  const Point &getOrientation() const;
  //  void setOrientation(const Point &);

  void addTrace(std::unique_ptr<SeismTrace>);

  unsigned getTracesNumber() const;
  const std::vector<std::unique_ptr<SeismTrace>> &getTraces() const;

  void addWavePick(SeismWavePick::Type, const SeismWavePick &);
  void removeWavePick(SeismWavePick::Type);
  const SeismWavePick &getWavePick(SeismWavePick::Type) const;
  const std::map<SeismWavePick::Type, SeismWavePick> &getWavePicks() const;

  QJsonObject &writeToJson(QJsonObject &) const;

signals:
  void changed();

private:
  //  QUuid _wellUuid;
  QUuid _receiverUuid;

  float _sampleInterval{0.0};
  //  int _pWaveArrival{0};
  //  int _sWaveArrival{0};

  float _maxValue{-1.0};

  //  Point _orientation{0.0, 0.0, 0.0};

  //  const std::unique_ptr<SeismReceiver> &_receiver;

  std::vector<std::unique_ptr<SeismTrace>> _traces;

  std::map<SeismWavePick::Type, SeismWavePick> _wavePicks_map;
};

} // namespace Data
