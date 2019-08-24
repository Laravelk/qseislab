#include "seismreciever.h"

#include <QJsonArray>

namespace Data {

SeismReciever::SeismReciever() {}

SeismReciever::SeismReciever(const QJsonObject &json) {
  std::string err_msg;

  if (json.contains("channelNum")) {
    _channelNum = json["channelNum"].toInt();
  } else {
    err_msg += "::channelNum : not found\n";
  }

  if (json.contains("Location")) {
    QJsonArray locationArray(json["Location"].toArray());
    if (3 == locationArray.size()) {
      _location = {locationArray[0].toDouble(), locationArray[1].toDouble(),
                   locationArray[2].toDouble()};
    } else {
      err_msg += "::Location : is not three-dimensional\n";
    }
  } else {
    err_msg += "::Location : not found\n";
  }

  if (!err_msg.empty()) {
    throw std::runtime_error(err_msg);
  }
}

void SeismReciever::setUuid(const QUuid &uuid) { _uuid = uuid; }

const QUuid &SeismReciever::getUuid() const { return _uuid; }

void SeismReciever::setName(const QString &name) { _name = name; }

const QString &SeismReciever::getName() const { return _name; }

void SeismReciever::setLocation(const Point &location) { _location = location; }

const Point &SeismReciever::getLocation() const { return _location; }

void SeismReciever::setChannelNum(int channelNum) { _channelNum = channelNum; }

int SeismReciever::getChannelNum() const { return _channelNum; }

QJsonObject &SeismReciever::writeToJson(QJsonObject &json) {
  json["channelNum"] = _channelNum;
  QJsonArray locationArray;
  locationArray.append(static_cast<double>(std::get<0>(_location)));
  locationArray.append(static_cast<double>(std::get<1>(_location)));
  locationArray.append(static_cast<double>(std::get<2>(_location)));
  json["Location"] = locationArray;
  return json;
}

} // namespace Data
