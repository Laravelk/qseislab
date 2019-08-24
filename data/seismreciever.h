#pragma once

#include "data/seism_data_type.h"

#include <QJsonObject>
#include <QUuid>

namespace Data {

class SeismReciever {
public:
  explicit SeismReciever();
  explicit SeismReciever(const QJsonObject &) noexcept(false);

  void setUuid(const QUuid &);
  const QUuid &getUuid() const;

  void setName(const QString &);
  const QString &getName() const;

  void setLocation(const Point &);
  const Point &getLocation() const;

  void setChannelNum(int);
  int getChannelNum() const;

  QJsonObject &writeToJson(QJsonObject &);

private:
  QUuid _uuid;

  QString _name;

  Point _location{0, 0, 0};

  int _channelNum{0};
};

} // namespace Data
