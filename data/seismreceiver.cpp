#include "seismreceiver.h"

#include <QJsonArray>

namespace Data {

SeismReceiver::SeismReceiver() : _uuid(QUuid::createUuid()) {}

SeismReceiver::SeismReceiver(const QJsonObject &json) {
  std::string err_msg;

  if (json.contains("uuid")) {
    _uuid = json["uuid"].toString();
  } else {
    err_msg += "::uuid : not found\n";
  }

  if (json.contains("name")) {
    _name = json["name"].toString();
  } else {
    err_msg += "::name : not found\n";
  }

  if (json.contains("receiverNum")) {
    _receiverNum = json["receiverNum"].toInt();
  } else {
    err_msg += "::receiverNum : not found\n";
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

  if (json.contains("on")) {
    _on = json["on"].toBool();
  } else {
    err_msg += "::on : not found\n";
  }

  if (json.contains("type")) {
    _type = json["type"].toInt();
  } else {
    err_msg += "::type : not found\n";
  }

  if (json.contains("gain")) {
    _gain = json["gain"].toInt();
  } else {
    err_msg += "::gain : not found\n";
  }

  if (json.contains("sensitivity")) {
    _sensitivity = json["sensitivity"].toInt();
  } else {
    err_msg += "::sensitivity : not found\n";
  }

  if (json.contains("vMax")) {
    _vMax = json["vMax"].toInt();
  } else {
    err_msg += "::vMax : not found\n";
  }

  if (json.contains("lowFreq")) {
    _lowFreq = json["lowFreq"].toInt();
  } else {
    err_msg += "::lowFreq : not found\n";
  }

  if (json.contains("highFreq")) {
    _highFreq = json["highFreq"].toInt();
  } else {
    err_msg += "::highFreq : not found\n";
  }

  if (json.contains("wellReceiverNum")) {
    _wellReceiverNum = json["wellReceiverNum"].toInt();
  } else {
    err_msg += "::wellReceiverNum : not found\n";
  }

  if (json.contains("Channels")) {
    QJsonArray channelsArray(json["Channels"].toArray());
    int idx = 0;
    for (auto objChannel : channelsArray) {
      try {
        auto channel =
            std::make_shared<SeismChannelReceiver>(objChannel.toObject());
        _channels.push_back(std::move(channel));
      } catch (const std::runtime_error &err) {
        err_msg += "Channel (idx: " + std::to_string(idx) + ")\n";
        err_msg += err.what();
      }
      ++idx;
    }
  } else {
    err_msg += "::Channels : not found\n";
  }

  if (!err_msg.empty()) {
    throw std::runtime_error(err_msg);
  }
}

SeismReceiver::SeismReceiver(const SeismReceiver &other)
    : _uuid(other._uuid), _name(other._name), _receiverNum(other._receiverNum),
      _location(other._location), _on(other._on), _type(other._type),
      _gain(other._gain), _sensitivity(other._sensitivity), _vMax(other._vMax),
      _lowFreq(other._lowFreq), _highFreq(other._highFreq),
      _wellReceiverNum(other._wellReceiverNum) {
  for (auto &channel : other._channels) {
    _channels.push_back(std::make_unique<SeismChannelReceiver>(*channel));
  }
}

// void SeismReceiver::setUuid(const QUuid &uuid) { _uuid = uuid; }

const QUuid &SeismReceiver::getUuid() const { return _uuid; }

void SeismReceiver::setName(const QString &name) { _name = name; }

const QString &SeismReceiver::getName() const { return _name; }

void SeismReceiver::setReceiverNum(int receiverNum) {
  _receiverNum = receiverNum;
}

int SeismReceiver::getReceiverNum() const { return _receiverNum; }

void SeismReceiver::setLocation(const Point &location) { _location = location; }

const Point &SeismReceiver::getLocation() const { return _location; }

void SeismReceiver::setOn(bool on) { _on = on; }

int SeismReceiver::getOn() const { return _on; }

void SeismReceiver::setType(int type) { _type = type; }

int SeismReceiver::getType() const { return _type; }

void SeismReceiver::setGain(int gain) { _gain = gain; }

int SeismReceiver::getGain() const { return _gain; }

void SeismReceiver::setSensitivity(int sensitivity) {
  _sensitivity = sensitivity;
}

int SeismReceiver::getSensitivity() const { return _sensitivity; }

void SeismReceiver::setVMax(int vMax) { _vMax = vMax; }

int SeismReceiver::getVMax() const { return _vMax; }

void SeismReceiver::setLowFreq(int lowFreq) { _lowFreq = lowFreq; }

int SeismReceiver::getLowFreq() const { return _lowFreq; }

void SeismReceiver::setHighFreq(int highFreq) { _highFreq = highFreq; }

int SeismReceiver::getHighFreq() const { return _highFreq; }

void SeismReceiver::setWellReceiverNum(int wellReceiverNum) {
  _wellReceiverNum = wellReceiverNum;
}

int SeismReceiver::getWellReceiverNum() const { return _wellReceiverNum; }

int SeismReceiver::getChannelAmount() const {
  return static_cast<int>(_channels.size());
}

void SeismReceiver::addChannel(
    const std::shared_ptr<SeismChannelReceiver> &channel) {
  _channels.push_back(channel);
}

const std::vector<std::shared_ptr<SeismChannelReceiver>> &
SeismReceiver::getChannels() const {
  return _channels;
}

QJsonObject &SeismReceiver::writeToJson(QJsonObject &json) {
  json["uuid"] = _uuid.toString();
  json["name"] = _name;
  json["receiverNum"] = _receiverNum;
  QJsonArray locationArray;
  locationArray.append(static_cast<double>(std::get<0>(_location)));
  locationArray.append(static_cast<double>(std::get<1>(_location)));
  locationArray.append(static_cast<double>(std::get<2>(_location)));
  json["Location"] = locationArray;
  json["on"] = _on;
  json["type"] = _type;
  json["gain"] = _gain;
  json["sensitivity"] = _sensitivity;
  json["vMax"] = _vMax;
  json["lowFreq"] = _lowFreq;
  json["highFreq"] = _highFreq;
  json["wellReceiverNum"] = _wellReceiverNum;
  QJsonArray channelsArray;
  QJsonObject channelObj;
  for (auto &channel : _channels) {
    channelsArray.append(channel->writeToJson(channelObj));
  }
  json["Channels"] = channelsArray;
  return json;
}

} // namespace Data
