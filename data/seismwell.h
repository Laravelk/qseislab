#pragma once

#include "data/seismreceiver.h"
#include "seism_data_type.h"

#include <QDir>
#include <QJsonObject>
#include <QUuid>

#include <vector>

namespace Data {
class SeismWell {
public:
  static const QString _default_path;

  explicit SeismWell();
  explicit SeismWell(const QJsonObject &, const QDir &) noexcept(false);

  explicit SeismWell(const SeismWell &);

  const QUuid &getUuid() const;

  void setName(const QString &);
  const QString &getName() const;

  int getPointsAmount() const;

  void addPoint(Point);
  const Point &getPoint(int);
  const std::vector<Point> &getPoints();

  void addReceiver(const std::shared_ptr<Data::SeismReceiver> &);
  bool removeReceiver(const QUuid &);
  int getReceiversAmount() const;
  const std::list<std::shared_ptr<Data::SeismReceiver>> &
  getReceivers(); // TODO: re-see
  void removeAllReceivers();

  QJsonObject &writeToJson(QJsonObject &, const QDir &) noexcept(false);

private:
  QUuid _uuid;

  QString _path;
  QString _name;
  std::vector<Point> _points;
  std::list<std::shared_ptr<Data::SeismReceiver>> _receivers;
  //  std::map<QUuid, std::unique_ptr<Data::SeismReceiver>> _receivers_map;
};

} // namespace Data
