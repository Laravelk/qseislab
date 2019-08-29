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

  if (!err_msg.empty()) {
    throw std::runtime_error(err_msg);
  }
}

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

QJsonObject &SeismChannelReceiver::writeToJson(QJsonObject &json) {
  json["name"] = _name;
  json["channelNum"] = _channelNum;
  json["axisNum"] = _axisNum;
  QJsonArray orientationArray;
  orientationArray.append(static_cast<double>(std::get<0>(_orientation)));
  orientationArray.append(static_cast<double>(std::get<1>(_orientation)));
  orientationArray.append(static_cast<double>(std::get<2>(_orientation)));
  json["Orientation"] = orientationArray;
  return json;
}

} // namespace Data
