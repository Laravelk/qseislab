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
  class Info {
  public:
    Info();

    void setStampDateTime(const QDateTime &);
    const QDateTime &getStampDateTime() const;

    void setSampleInterval(float);
    float getSampleInterval() const;

  private:
    QDateTime stampDateTime;
    float sampleInterval{0.0};

    friend class SeismComponent;
  };
  explicit SeismComponent(const QUuid &);

  explicit SeismComponent(const QJsonObject &json) noexcept(false);

  explicit SeismComponent(const SeismComponent &);

  const QUuid &getReceiverUuid() const;
  //  const QDateTime &getStampDateTime() const;
  //  void setStampDateTime(const QDateTime &);

  //  float getSampleInterval() const;
  //  void setSampleInterval(float);

  void setInfo(const Info &);
  const Info &getInfo() const;
  Info &getInfo();

  float getMaxValue() const;

  void addTrace(const std::shared_ptr<SeismTrace> &);

  int getTraceSize() const;
  int getTracesAmount() const;
  const std::vector<std::shared_ptr<SeismTrace>> &getTraces() const;

  void addWavePick(SeismWavePick);
  void removeWavePick(const SeismWavePick::Type);
  bool containsWavePickBy(const SeismWavePick::Type) const;
  const SeismWavePick &getWavePick(SeismWavePick::Type) const;
  const std::map<SeismWavePick::Type, SeismWavePick> &getWavePicks() const;

  QJsonObject &writeToJson(QJsonObject &) const;

signals:
  void infoChanged() const;
  void dataChanged() const;

private:
  QUuid _receiverUuid;

  Info _info;

  float _maxValue{-1.0};
  int _tracesSize{-1};
  std::vector<std::shared_ptr<SeismTrace>> _traces;

  std::map<SeismWavePick::Type, SeismWavePick> _wavePicks_map;
};

} // namespace Data
