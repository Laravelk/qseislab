#pragma once

#include "seism_data_type.h"
#include "seismreceiver.h"
#include "seismtrace.h"

#include <QJsonObject>

#include <memory>
#include <vector>

namespace Data {
class SeismTrace;
class SeismComponent {
public:
  explicit SeismComponent(const std::unique_ptr<SeismReceiver> &);
  explicit SeismComponent(
      const QJsonObject &json, const std::unique_ptr<SeismReceiver> &,
      std::vector<std::pair<uint32_t, std::unique_ptr<float[]>>>
          &data) noexcept(false);

  int getPWaveArrival() const;
  void setPWaveArrival(int);

  int getSWaveArrival() const;
  void setSWaveArrival(int);

  float getMaxValue() const;

  const std::unique_ptr<SeismReceiver> &getReceiver() const;

  //  const Point &getOrientation() const;
  //  void setOrientation(const Point &);

  void addTrace(std::unique_ptr<SeismTrace>);

  unsigned getTracesNumber() const;
  const std::vector<std::unique_ptr<SeismTrace>> &getTraces() const;

  QJsonObject &writeToJson(QJsonObject &) const;

private:
  int _pWaveArrival{0};
  int _sWaveArrival{0};

  float _maxValue{-1.0};

  //  Point _orientation{0.0, 0.0, 0.0};

  const std::unique_ptr<SeismReceiver> &_receiver;

  std::vector<std::unique_ptr<SeismTrace>> _traces;
};

} // namespace Data
