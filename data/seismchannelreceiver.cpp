#include "seismchannelreceiver.h"

#include <QJsonArray>

namespace Data {
SeismChannelReceiver::SeismChannelReceiver() {}

SeismChannelReceiver::SeismChannelReceiver(const QJsonObject &json) {
  std::string err_msg;

  if (json.contains("name")) {
    _name = json["name"].toString();
  } else {
    err_msg += "::name : not found\n";
  }

  if (json.contains("channelNum")) {
    _channelNum = json["channelNum"].toInt();
  } else {
    err_msg += "::channelNum : not found\n";
  }

  if (json.contains("axisNum")) {
    _axisNum = json["axisNum"].toInt();
  } else {
    err_msg += "::axisNum : not found\n";
  }

  if (json.contains("Orientation")) {
    QJsonArray orientationArray(json["Orientation"].toArray());
    if (3 == orientationArray.size()) {
      _orientation = {orientationArray[0].toDouble(),
                      orientationArray[1].toDouble(),
                      orientationArray[2].toDouble()};
    } else {
      err_msg += "::Orientation : is not three-dimensional\n";
    }
  } else {
    err_msg += "::Orientation : not found\n";
  }

  if (json.contains("motion")) {
    _motion = json["motion"].toInt();
  } else {
    err_msg += "::motion : not found\n";
  }

  if (json.contains("pStationCorrection")) {
    _pStationCorrection = json["pStationCorrection"].toInt();
  } else {
    err_msg += "::pStationCorrection : not found\n";
  }

  if (json.contains("sStationCorrection")) {
    _sStationCorrection = json["sStationCorrection"].toInt();
  } else {
    err_msg += "::sStationCorrection : not found\n";
  }

  if (json.contains("wellChannelNum")) {
    _wellChannelNum = json["wellChannelNum"].toInt();
  } else {
    err_msg += "::wellChannelNum : not found\n";
  }

  if (!err_msg.empty()) {
    throw std::runtime_error(err_msg);
  }
}

SeismChannelReceiver::SeismChannelReceiver(const SeismChannelReceiver &other)
    : _name(other._name), _channelNum(other._channelNum),
      _axisNum(other._axisNum), _orientation(other._orientation),
      _motion(other._motion), _pStationCorrection(other._pStationCorrection),
      _sStationCorrection(other._sStationCorrection),
      _wellChannelNum(other._wellChannelNum) {}

const QString &SeismChannelReceiver::getName() const { return _name; }

void SeismChannelReceiver::setName(const QString &name) { _name = name; }

int SeismChannelReceiver::getChannelNum() const { return _channelNum; }

void SeismChannelReceiver::setChannelNum(int channelNum) {
  _channelNum = channelNum;
}

int SeismChannelReceiver::getAxisNum() const { return _axisNum; }

void SeismChannelReceiver::setAxisNum(int axisNum) { _axisNum = axisNum; }

const Point &SeismChannelReceiver::getOrientation() const {
  return _orientation;
}

void SeismChannelReceiver::setOrientation(const Point &orientation) {
  _orientation = orientation;
}

int SeismChannelReceiver::getMotion() const { return _motion; }

void SeismChannelReceiver::setMotion(int motion) { _motion = motion; }

int SeismChannelReceiver::getPStationCorrection() const {
  return _pStationCorrection;
}

void SeismChannelReceiver::setPStationCorrection(int pStationCorrection) {
  _pStationCorrection = pStationCorrection;
}

int SeismChannelReceiver::getSStationCorrection() const {
  return _sStationCorrection;
}

void SeismChannelReceiver::setSStationCorrection(int sStationCorrection) {
  _sStationCorrection = sStationCorrection;
}

int SeismChannelReceiver::getWellChannelNum() const { return _wellChannelNum; }

void SeismChannelReceiver::setWellChannelNum(int wellChannelNum) {
  _wellChannelNum = wellChannelNum;
}

QJsonObject &SeismChannelReceiver::writeToJson(QJsonObject &json) {
  json["name"] = _name;
  json["channelNum"] = _channelNum;
  json["axisNum"] = _axisNum;
  QJsonArray orientationArray;
  orientationArray.append(static_cast<double>(std::get<0>(_orientation)));
  orientationArray.append(static_cast<double>(std::get<1>(_orientation)));
  orientationArray.append(static_cast<double>(std::get<2>(_orientation)));
  json["Orientation"] = orientationArray;
  json["motion"] = _motion;
  json["pStationCorrection"] = _pStationCorrection;
  json["sStationCorrection"] = _sStationCorrection;
  json["wellChannelNum"] = _wellChannelNum;
  return json;
}

} // namespace Data
