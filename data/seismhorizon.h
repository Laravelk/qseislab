#pragma once

#include <QDir>
#include <QJsonObject>
#include <QUuid>

#include <tuple>
#include <vector>

namespace Data {
class SeismHorizon {
public:
  typedef std::tuple<float, float, float> SeismPoint;

  static const QString _default_path;

  explicit SeismHorizon();
  explicit SeismHorizon(const QJsonObject &, const QDir &) noexcept(false);

  void setName(const QString &);
  const QString &getName() const;

  int getPointsNumber() const;

  void addPoint(const SeismPoint &);
  const SeismPoint &getPoint(int);
  const std::vector<SeismPoint> &getPoints();

  void setUuid(const QUuid &);
  const QUuid &getUuid() const;

  QJsonObject &writeToJson(QJsonObject &, const QDir &) noexcept(false);

private:
  QString _path;
  QString _name;

  QUuid _uuid;

  std::vector<SeismPoint> _points;
};

} // namespace Data
