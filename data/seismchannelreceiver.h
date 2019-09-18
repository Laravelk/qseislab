#pragma once

#include "seism_data_type.h"

#include <QJsonObject>
#include <QUuid>

namespace Data {
class SeismChannelReceiver {
public:
  explicit SeismChannelReceiver();
  explicit SeismChannelReceiver(const QJsonObject &) noexcept(false);

  explicit SeismChannelReceiver(const SeismChannelReceiver &);

  const QString &getName() const;
  void setName(const QString &);

  int getChannelNum() const;
  void setChannelNum(int);

  int getAxisNum() const;
  void setAxisNum(int);

  const Point &getOrientation() const;
  void setOrientation(const Point &);

  int getMotion() const;
  void setMotion(int);

  int getPStationCorrection() const;
  void setPStationCorrection(int);

  int getSStationCorrection() const;
  void setSStationCorrection(int);

  int getWellChannelNum() const;
  void setWellChannelNum(int);

  QJsonObject &writeToJson(QJsonObject &);

private:
  QString _name;
  int _channelNum;
  int _axisNum;
  Point _orientation;
  int _motion;
  int _pStationCorrection;
  int _sStationCorrection;
  int _wellChannelNum;
};

} // namespace Data
