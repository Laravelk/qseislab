#pragma once

#include "seismtrace.h"

#include <QJsonObject>

#include <memory>
#include <vector>

namespace Data {
class SeismComponent {
public:
  explicit SeismComponent();
  explicit SeismComponent(
      const QJsonObject &json,
      std::vector<std::pair<uint32_t, std::unique_ptr<float[]>>>
          &data) noexcept(false);

  int getPWaveArrival() const;
  void setPWaveArrival(int);

  int getSWaveArrival() const;
  void setSWaveArrival(int);

  float getMaxValue() const;

  void addTrace(std::unique_ptr<SeismTrace>);

  unsigned getTracesNumber() const;
  const std::vector<std::unique_ptr<SeismTrace>> &getTraces() const;

  QJsonObject &writeToJson(QJsonObject &) const;

private:
  int _pWaveArrival{0};
  int _sWaveArrival{0};

  float _maxValue{-1.0};

  std::vector<std::unique_ptr<SeismTrace>> _traces;
};

} // namespace Data
