#pragma once

#include "seism_data_type.h"
#include "seismcomponent.h"

#include <memory>

#include <QDateTime>
#include <QDir>
#include <QJsonObject>
#include <QUuid>

namespace Data {
class SeismEvent {
public:
  static const QString _default_path;

  explicit SeismEvent();
  explicit SeismEvent(const QJsonObject &,
                      std::list<std::unique_ptr<SeismReceiver>> &,
                      const QDir &) noexcept(false);

  int getComponentNumber() const;
  void addComponent(std::unique_ptr<SeismComponent>);
  const std::list<std::unique_ptr<SeismComponent>> &getComponents() const;

  void setDateTime(const QDateTime &);
  const QDateTime &getDateTime() const;

  void setUuid(const QUuid &);
  const QUuid &getUuid() const;

  void process();
  bool isProcessed() const;
  const Point &getLocation() const;

  QJsonObject &writeToJson(QJsonObject &, const QDir &) noexcept(false);

private:
  QString _path;

  QDateTime _dateTime;

  QUuid _uuid;

  bool _isProcessed{false};
  Point _location{0, 0, 0};

  std::list<std::unique_ptr<SeismComponent>> _components;
};

} // namespace Data
