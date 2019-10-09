#include "seismcomponent.h"

#include <QJsonArray>

namespace Data {
SeismComponent::SeismComponent(const QUuid &receiverUuid)
    : _receiverUuid(receiverUuid) {}

// SeismComponent::SeismComponent(
//    const QJsonObject &json,
//    std::vector<std::pair<uint32_t, std::unique_ptr<float[]>>> &data) {

//  std::string err_msg;

//  if (json.contains("pWaveArrival")) {
//    _pWaveArrival = json["pWaveArrival"].toInt();
//  } else {
//    err_msg += "::pWaveArrival : not found\n";
//  }

//  if (json.contains("sWaveArrival")) {
//    _sWaveArrival = json["sWaveArrival"].toInt();
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
//      err_msg += "::data : traces-size in json-file doesn`t match traces-size
//      "
//                 "in bin-file\n";
//    }
//  } else {
//    err_msg += "::Traces : not found\n";
//  }

//  if (!err_msg.empty()) {
//    err_msg += "\n";
//    throw std::runtime_error(err_msg);
//  }

//  for (unsigned i = 0; i < _traces.size(); ++i) {
//    float traceMaxValue = (_traces[i])->getMaxValue();
//    if (traceMaxValue > _maxValue) {
//      _maxValue = traceMaxValue;
//    }
//  }
//}

SeismComponent::SeismComponent(const QJsonObject &json) {

  std::string err_msg;

  //  if (json.contains("wellUuid")) {
  //    _wellUuid = json["wellUuid"].toString();
  //  } else {
  //    err_msg += "::wellUuid : not found\n";
  //  }

  if (json.contains("receiverUuid")) {
    _receiverUuid = json["receiverUuid"].toString();
  } else {
    err_msg += "::receiverUuid : not found\n";
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
  //      traces-size "
  //                 "in bin-file\n";
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
    : //    _wellUuid(other._wellUuid),
      _receiverUuid(other._receiverUuid),
      //    _pWaveArrival(other._pWaveArrival),
      //      _sWaveArrival(other._sWaveArrival),
      _maxValue(other._maxValue) {
  for (auto &trace : other._traces) {
    _traces.push_back(std::make_unique<SeismTrace>(*trace));
  }
}

// const QUuid &SeismComponent::getWellUuid() const { return _wellUuid; }

const QUuid &SeismComponent::getReceiverUuid() const { return _receiverUuid; }

float SeismComponent::getSampleInterval() const { return _sampleInterval; }

void SeismComponent::setSampleInterval(float sampleInterval) {
  _sampleInterval = sampleInterval;
}

// int SeismComponent::getPWaveArrival() const { return _pWaveArrival; }

// void SeismComponent::setPWaveArrival(int pWaveArrival) {
//  _pWaveArrival = pWaveArrival;
//}

// int SeismComponent::getSWaveArrival() const { return _sWaveArrival; }

// void SeismComponent::setSWaveArrival(int sWaveArrival) {
//  _sWaveArrival = sWaveArrival;
//}

float SeismComponent::getMaxValue() const { return _maxValue; }

// const std::unique_ptr<SeismReceiver> &SeismComponent::getReceiver() const {
//  return _receiver;
//}

// const Point &SeismComponent::getOrientation() const {
//  //  return _receiver->getOrientation();
//}

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

void SeismComponent::addWavePick(SeismWavePick::Type type,
                                 const SeismWavePick &wavePick) {
  _wavePicks_map[type] = wavePick;
  emit changed();
}

void SeismComponent::removeWavePick(SeismWavePick::Type type) {
  _wavePicks_map.erase(type);
  emit changed();
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
  //  json["wellUuid"] = _wellUuid.toString();
  //  json["pWaveArrival"] = _pWaveArrival;
  //  json["sWaveArrival"] = _sWaveArrival;

  QJsonArray tracesArray;
  QJsonObject traceObj;
  for (const std::unique_ptr<SeismTrace> &trace : _traces) {
    tracesArray.append(trace->writeToJson(traceObj));
  }

  json["Traces"] = tracesArray;

  return json;
}

} // namespace Data
