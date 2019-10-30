#include "seismcomponent.h"

#include <QJsonArray>

namespace Data {
SeismComponent::SeismComponent(const QUuid &receiverUuid)
    : _receiverUuid(receiverUuid) {}

SeismComponent::SeismComponent(const QJsonObject &json) {

  std::string err_msg;

  if (json.contains("receiverUuid")) {
    _receiverUuid = json["receiverUuid"].toString();
  } else {
    err_msg += "::receiverUuid : not found\n";
  }

  //  if (json.contains("stampDateTime")) {
  //    _stampDateTime = QDateTime::fromString(json["stampTime"].toString(),
  //                                       "dd.MM.yyyy hh:mm:ss:zzz");
  //  } else {
  //    err_msg += "::stampTime : not found\n";
  //  }

  if (json.contains("sampleInterval")) {
    _sampleInterval = static_cast<float>(json["sampleInterval"].toDouble());
  } else {
    err_msg += "::sampleInterval : not found\n";
  }

  if (json.contains("Waves")) {
    QJsonArray wavesArray(json["Waves"].toArray());
    int idx = 0;
    for (auto objWave : wavesArray) {
      try {
        auto seismWave = SeismWavePick(objWave.toObject());
        auto type = seismWave.getType();
        _wavePicks_map[type] = seismWave;
      } catch (std::runtime_error &err) {
        err_msg += "Waves (idx: " + std::to_string(idx) + ")\n";
        err_msg += err.what();
      }
      ++idx;
    }
  } else {
    err_msg += "::Waves : not found\n";
  }

  // TODO: refactor -> wavePicks_map
  //  if (json.contains("pWaveArrival")) {
  //        _pWaveArrival = json["pWaveArrival"].toInt();
  //  } else {
  //    err_msg += "::pWaveArrival : not found\n";
  //  }

  // TODO: refactor -> wavePicks_map
  //  if (json.contains("sWaveArrival")) {
  //        _sWaveArrival = json["sWaveArrival"].toInt();
  //  } else {
  //    err_msg += "::sWaveArrival : not found\n";
  //  }

  //  if (json.contains("Traces")) {
  //    QJsonArray tracesArray(json["Traces"].toArray());

  //    if (static_cast<int>(data.size()) == tracesArray.count()) {
  //      unsigned idx = 0;
  //      for (auto objTrace : tracesArray) {
  //        try {
  //          auto seismTrace =
  //              std::make_unique<SeismTrace>(objTrace.toObject(), data[idx]);
  //          _traces.push_back(std::move(seismTrace));
  //        } catch (std::runtime_error &err) {
  //          err_msg += "Trace (idx: " + std::to_string(idx) + ")\n";
  //          err_msg += err.what();
  //        }
  //        ++idx;
  //      }
  //    } else {
  //      err_msg += "::data : traces-size in json-file doesn`t match
  //                 traces -
  //                 size "
  //                      "in bin-file\n";
  //    }
  //  } else {
  //    err_msg += "::Traces : not found\n";
  //  }

  if (!err_msg.empty()) {
    err_msg += "\n";
    throw std::runtime_error(err_msg);
  }

  for (unsigned i = 0; i < _traces.size(); ++i) {
    float traceMaxValue = (_traces[i])->getMaxValue();
    if (traceMaxValue > _maxValue) {
      _maxValue = traceMaxValue;
    }
  }
}

SeismComponent::SeismComponent(const SeismComponent &other)
    : _receiverUuid(other._receiverUuid),
      _sampleInterval(other._sampleInterval), _maxValue(other._maxValue),
      _wavePicks_map(other._wavePicks_map) {
  for (auto &trace : other._traces) {
    _traces.push_back(std::make_unique<SeismTrace>(*trace));
  }
}

const QUuid &SeismComponent::getReceiverUuid() const { return _receiverUuid; }

const QDateTime &SeismComponent::getStampDateTime() const {
  return _stampDateTime;
}

void SeismComponent::setStampDateTime(const QDateTime &dateTime) {
  _stampDateTime = dateTime;
}

float SeismComponent::getSampleInterval() const { return _sampleInterval; }

void SeismComponent::setSampleInterval(float sampleInterval) {
  _sampleInterval = sampleInterval;
}

float SeismComponent::getMaxValue() const { return _maxValue; }

void SeismComponent::addTrace(std::unique_ptr<SeismTrace> trace) {
  if (_maxValue < trace->getMaxValue()) {
    _maxValue = trace->getMaxValue();
  }

  _traces.push_back(std::move(trace));
}

unsigned SeismComponent::getTracesAmount() const {
  return static_cast<unsigned>(_traces.size());
}

const std::vector<std::unique_ptr<SeismTrace>> &
SeismComponent::getTraces() const {
  return _traces;
}

void SeismComponent::addWavePick(const SeismWavePick wavePick) {
  _wavePicks_map[wavePick.getType()] = wavePick;
  emit changed();
}

void SeismComponent::removeWavePick(const SeismWavePick::Type type) {
  _wavePicks_map.erase(type);
  emit changed();
}

bool SeismComponent::containsWavePickBy(const SeismWavePick::Type type) const {
  return _wavePicks_map.end() != _wavePicks_map.find(type);
}

const SeismWavePick &
SeismComponent::getWavePick(SeismWavePick::Type type) const {
  return _wavePicks_map.at(type);
}

const std::map<SeismWavePick::Type, SeismWavePick> &
SeismComponent::getWavePicks() const {
  return _wavePicks_map;
}

QJsonObject &SeismComponent::writeToJson(QJsonObject &json) const {
  json["receiverUuid"] = _receiverUuid.toString();
  //  json["stampTime"] = _stampTime.toString("dd.MM.yyyy hh:mm:ss:zzz");
  json["sampleInterval"] = static_cast<double>(_sampleInterval);

  QJsonArray wavesArray;
  QJsonObject waveObj;
  for (auto &type_wave : _wavePicks_map) {
    wavesArray.append(type_wave.second.writeToJson(waveObj));
  }

  json["Waves"] = wavesArray;

  return json;
}

} // namespace Data
