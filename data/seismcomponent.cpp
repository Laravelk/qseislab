#include "seismcomponent.h"

#include <QJsonArray>

namespace Data {
SeismComponent::SeismComponent() {}

SeismComponent::SeismComponent(
    const QJsonObject &json,
    std::vector<std::pair<uint32_t, std::unique_ptr<float[]>>> &data) {

  //    if( !(json.contains("Traces")) ) {
  if (!(json.contains("pWaveArrival") && json.contains("sWaveArrival"))) {
    throw std::runtime_error("Not found json-field (SeismComponent)");
  }

  _pWaveArrival = json["pWaveArrival"].toInt();
  _sWaveArrival = json["sWaveArrival"].toInt();

  QJsonArray tracesArray(json["Traces"].toArray());

  if (static_cast<int>(data.size()) != tracesArray.count()) {
    throw std::runtime_error("Traces-size in bin-file does not match "
                             "traces-size in json-file (SeismComponent)");
  }

  unsigned i = 0;
  for (auto objTrace : tracesArray) {
    auto seismTrace =
        std::make_unique<SeismTrace>(objTrace.toObject(), data[i++]);
    _traces.push_back(std::move(seismTrace));
  }

  for (unsigned i = 0; i < _traces.size(); ++i) {
    float traceMaxValue = (_traces[i])->getMaxValue();
    if (traceMaxValue > _maxValue) {
      _maxValue = traceMaxValue;
    }
  }

  //    if ( json.contains("maxValue") ) {
  //        float jmaxValue = static_cast<float>(json["maxValue"].toDouble());
  //        if ( jmaxValue != _maxValue) {
  //            //TODO: notify
  //        }
  //    }
  //    else {
  //        //TODO: notify
  //    }
}

int SeismComponent::getPWaveArrival() const { return _pWaveArrival; }

void SeismComponent::setPWaveArrival(int pWaveArrival) {
  _pWaveArrival = pWaveArrival;
}

int SeismComponent::getSWaveArrival() const { return _sWaveArrival; }

void SeismComponent::setSWaveArrival(int sWaveArrival) {
  _sWaveArrival = sWaveArrival;
}

float SeismComponent::getMaxValue() const { return _maxValue; }

void SeismComponent::addTrace(std::unique_ptr<SeismTrace> trace) {
  if (_maxValue < trace->getMaxValue()) {
    _maxValue = trace->getMaxValue();
  }

  _traces.push_back(std::move(trace));
}

unsigned SeismComponent::getTracesNumber() const {
  return static_cast<unsigned>(_traces.size());
}

const std::vector<std::unique_ptr<SeismTrace>> &
SeismComponent::getTraces() const {
  return _traces;
}

QJsonObject &SeismComponent::writeToJson(QJsonObject &json) const {
  //    json["maxValue"] = static_cast<double>(_maxValue);
  json["pWaveArrival"] = _pWaveArrival;
  json["sWaveArrival"] = _sWaveArrival;

  QJsonArray tracesArray;
  QJsonObject traceObj;
  for (const std::unique_ptr<SeismTrace> &trace : _traces) {
    tracesArray.append(trace->writeToJson(traceObj));
  }

  json["Traces"] = tracesArray;

  return json;
}

} // namespace Data
