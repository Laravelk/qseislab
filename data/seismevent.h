#pragma once

#include "seism_data_type.h"
#include "seismcomponent.h"

#include <memory>

#include <QDateTime>
#include <QDir>
#include <QJsonObject>
#include <QUuid>

namespace Data {
class SeismWell;
class SeismEvent : public QObject {
  Q_OBJECT
public:
  static const QString _default_path;

  explicit SeismEvent();
  explicit SeismEvent(const QJsonObject &,
                      std::map<QUuid, std::unique_ptr<SeismWell>> &,
                      const QDir &) noexcept(false);

  explicit SeismEvent(const SeismEvent &);

  const QUuid &getUuid() const;

  void setName(const QString &);
  const QString &getName() const;

  //  void setAddedDateTime(const QDateTime &);
  const QDateTime &getAddedDateTime() const;

  void setType(int);   // TODO: remove
  int getType() const; // TODO: remove

  void setStampDateTime(const QDateTime &);
  const QDateTime &getStampDateTime() const;

  int getPickAmountByType(const SeismWavePick::Type) const;
  int getReceiverAmount() const;

  int getComponentAmount() const;
  void addComponent(std::unique_ptr<SeismComponent>);
  bool removeComponentByReceiverUuid(const QUuid &);
  const std::list<std::unique_ptr<SeismComponent>> &getComponents() const;

  void process();
  bool isProcessed() const;
  const Point &getLocation() const;

  QJsonObject &writeToJson(QJsonObject &, const QDir &) noexcept(false);

signals:
  void changed();

private:
  QUuid _uuid;
  QString _path;

  QString _name;
  QDateTime _addedDateTime;

  int _type = 0; // TODO: remove

  QDateTime _stampDateTime;
  bool _isProcessed{false};
  Point _location{0, 0, 0};
  std::list<std::unique_ptr<SeismComponent>> _components;
};

} // namespace Data
