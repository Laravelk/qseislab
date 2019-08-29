#pragma once

#include "data/seism_data_type.h"
#include "seismchannelReceiver.h"

#include <QJsonObject>
#include <QUuid>

#include <memory>
#include <vector>

namespace Data {
class SeismReceiver {
public:
  explicit SeismReceiver();
  explicit SeismReceiver(const QJsonObject &) noexcept(false);

  void setUuid(const QUuid &);
  const QUuid &getUuid() const;

  void setName(const QString &);
  const QString &getName() const;

  void setWellNum(int);
  int getWellNum() const;

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

  int getChannelNum() const;
  void addChannel(std::unique_ptr<Data::SeismChannelReceiver>);
  const std::vector<std::unique_ptr<Data::SeismChannelReceiver>> &
  getChannels() const;

  QJsonObject &writeToJson(QJsonObject &);

private:
  QUuid _uuid;

  QString _name;
  int _wellNum{-1};
  Point _location{0, 0, 0};
  bool _on{false};
  int _type;
  int _gain;
  int _sensitivity;
  int _vMax;
  int _lowFreq;
  int _highFreq;

  std::vector<std::unique_ptr<Data::SeismChannelReceiver>> _channels;
};

} // namespace Data
