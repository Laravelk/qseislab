#pragma once

#include "seism_data_type.h"

#include <QDir>
#include <QJsonObject>
#include <QUuid>

#include <vector>

namespace Data {
class SeismHorizon {
public:
  static const QString _default_path;

  explicit SeismHorizon();
  explicit SeismHorizon(const QJsonObject &, const QDir &) noexcept(false);

  void setName(const QString &);
  const QString &getName() const;

  int getPointsNumber() const;

  void addPoint(const Point &);
  const Point &getPoint(int);
  const std::vector<Point> &getPoints();

  void setUuid(const QUuid &);
  const QUuid &getUuid() const;

  void setNx(int);
  int getNx() const;

  void setNy(int);
  int getNy() const;

  QJsonObject &writeToJson(QJsonObject &, const QDir &) noexcept(false);

private:
  QString _path;
  QString _name;

  QUuid _uuid;

  int _Nx{0};
  int _Ny{0};

  std::vector<Point> _points;
};

} // namespace Data
