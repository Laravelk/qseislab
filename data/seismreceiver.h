#pragma once

#include "data/seism_data_type.h"
#include "seismchannelreceiver.h"

#include <QJsonObject>
#include <QUuid>

#include <memory>
#include <vector>

namespace Data {
class SeismReceiver {
public:
  explicit SeismReceiver();
  explicit SeismReceiver(const QJsonObject &) noexcept(false);

  explicit SeismReceiver(const SeismReceiver &);

  const QUuid &getUuid() const;

  void setName(const QString &);
  const QString &getName() const;

  void setReceiverNum(int);
  int getReceiverNum() const;

  void setLocation(const Point &);
  const Point &getLocation() const;

  void setOn(bool);
  int getOn() const;

  void setType(int);
  int getType() const;

  void setGain(int);
  int getGain() const;

  void setSensitivity(int);
  int getSensitivity() const;

  void setVMax(int);
  int getVMax() const;

  void setLowFreq(int);
  int getLowFreq() const;

  void setHighFreq(int);
  int getHighFreq() const;

  void setWellReceiverNum(int);
  int getWellReceiverNum() const;

  int getChannelAmount() const;
  void addChannel(const std::shared_ptr<Data::SeismChannelReceiver> &);
  const std::vector<std::shared_ptr<Data::SeismChannelReceiver>> &
  getChannels() const;

  QJsonObject &writeToJson(QJsonObject &);

private:
  QUuid _uuid;

  QString _name;
  int _receiverNum;
  Point _location{0, 0, 0};
  bool _on{false};
  int _type;
  int _gain;
  int _sensitivity;
  int _vMax;
  int _lowFreq;
  int _highFreq;
  int _wellReceiverNum;
  std::vector<std::shared_ptr<Data::SeismChannelReceiver>> _channels;
};

} // namespace Data
