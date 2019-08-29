#pragma once

#include "seism_data_type.h"

#include <QJsonObject>
#include <QUuid>

namespace Data {
class SeismChannelReceiver {
public:
  explicit SeismChannelReceiver();
  explicit SeismChannelReceiver(const QJsonObject &) noexcept(false);

  const QString &getName() const;
  void setName(const QString &);

  int getChannelNum() const;
  void setChannelNum(int);

  int getAxisNum() const;
  void setAxisNum(int);

  const Point &getOrientation() const;
  void setOrientation(const Point &);

  QJsonObject &writeToJson(QJsonObject &);

private:
  QString _name;
  int _channelNum;
  int _axisNum;
  Point _orientation;
};

} // namespace Data
