#pragma once

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

  void setName(const QString &);
  const QString &getName() const;

  int getPointsNumber() const;

  void addPoint(const Point &);
  const Point &getPoint(int);
  const std::vector<Point> &getPoints();

  void setUuid(const QUuid &);
  const QUuid &getUuid() const;

  QJsonObject &writeToJson(QJsonObject &, const QDir &) noexcept(false);

private:
  QString _path;
  QString _name;

  QUuid _uuid;

  std::vector<Point> _points;
};

} // namespace Data
