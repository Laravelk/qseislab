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
    _info.sampleInterval =
        static_cast<float>(json["sampleInterval"].toDouble());
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

  //  for (unsigned i = 0; i < _traces.size(); ++i) {
  //    float traceMaxValue = (_traces[i])->getMaxValue();
  //    if (traceMaxValue > _maxValue) {
  //      _maxValue = traceMaxValue;
  //    }
  //  }
}

SeismComponent::SeismComponent(const SeismComponent &other)
    : _receiverUuid(other._receiverUuid),
      //      _sampleInterval(other._sampleInterval),
      _info(other._info), _maxValue(other._maxValue), _traces(other._traces),
      _wavePicks_map(other._wavePicks_map) {}

const QUuid &SeismComponent::getReceiverUuid() const { return _receiverUuid; }

void SeismComponent::setInfo(const SeismComponent::Info &info) {
  _info = info;
  emit infoChanged();
}

const SeismComponent::Info &SeismComponent::getInfo() const { return _info; }

SeismComponent::Info &SeismComponent::getInfo() { return _info; }

const QDateTime &SeismComponent::getStampDateTime() const {
  return _info.stampDateTime;
}

// void SeismComponent::setStampDateTime(const QDateTime &dateTime) {
//  _stampDateTime = dateTime;
//}

float SeismComponent::getSampleInterval() const { return _info.sampleInterval; }

// void SeismComponent::setSampleInterval(float sampleInterval) {
//  _sampleInterval = sampleInterval;
//}

float SeismComponent::getMaxValue() const { return _maxValue; }

void SeismComponent::addTrace(const std::shared_ptr<SeismTrace> &trace) {
  if (_maxValue < trace->getMaxValue()) {
    _maxValue = trace->getMaxValue();
  }

  connect(trace.get(), &SeismTrace::changed, [this] { emit dataChanged(); });

  // TODO: добавление трасс разного размера
  // NOTE: пока что берется наибольщий размер, а другие добиваются нулями
  if (trace->getBufferSize() > _tracesSize) {
    _tracesSize = trace->getBufferSize();
    for (auto &_trace : _traces) {
      if (_trace->getBufferSize() < _tracesSize) {
        float *old_buffer = trace->getBuffer().get();
        float *new_buffer = new float[static_cast<unsigned long>(_tracesSize)];
        for (int i = 0; _tracesSize; ++i) {
          if (_trace->getBufferSize() > i) {
            new_buffer[i] = old_buffer[i];
          } else {
            new_buffer[i] = 0.0;
          }
        }

        old_buffer = nullptr;
        trace->setBuffer(static_cast<uint32_t>(_tracesSize), new_buffer);
      }
    }
  }

  _traces.push_back(trace);

  emit dataChanged();
}

int SeismComponent::getTraceSize() const { return _tracesSize; }

int SeismComponent::getTracesAmount() const {
  return static_cast<int>(_traces.size());
}

const std::vector<std::shared_ptr<SeismTrace>> &
SeismComponent::getTraces() const {
  return _traces;
}

void SeismComponent::addWavePick(const SeismWavePick wavePick) {
  _wavePicks_map[wavePick.getType()] = wavePick;
  emit dataChanged();
}

void SeismComponent::removeWavePick(const SeismWavePick::Type type) {
  _wavePicks_map.erase(type);
  emit dataChanged();
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
  json["sampleInterval"] = static_cast<double>(_info.sampleInterval);

  QJsonArray wavesArray;
  QJsonObject waveObj;
  for (auto &type_wave : _wavePicks_map) {
    wavesArray.append(type_wave.second.writeToJson(waveObj));
  }

  json["Waves"] = wavesArray;

  return json;
}

SeismComponent::Info::Info() {}

const QDateTime &SeismComponent::Info::getStampDateTime() const {
  return stampDateTime;
}

void SeismComponent::Info::setStampDateTime(const QDateTime &dateTime) {
  this->stampDateTime = dateTime;
}

float SeismComponent::Info::getSampleInterval() const { return sampleInterval; }

void SeismComponent::Info::setSampleInterval(float sampleInterval) {
  this->sampleInterval = sampleInterval;
}

} // namespace Data
