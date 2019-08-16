#pragma once

#include "seismevent.h"
#include "seismhorizon.h"

#include <QDateTime>
#include <QFileInfo>
#include <QJsonObject>
#include <QObject>

#include <map>
#include <memory>

namespace Data {
class SeismProject : public QObject {
  Q_OBJECT

public:
  explicit SeismProject(QObject *parent = nullptr);
  explicit SeismProject(const QJsonObject &, const QFileInfo &,
                        QObject *parent = nullptr) noexcept(false);

  bool exist() const;
  bool isSaved() const;

  QJsonObject &writeToJson(QJsonObject &, const QFileInfo &) noexcept(false);

  void setName(const QString &);
  const QString &getName() const;

  void setDate(const QDate &);
  void setTime(const QTime &);
  void setDateTime(const QDateTime &);
  const QDateTime &getDateTime() const;

  void setFileInfo(const QFileInfo &);
  const QFileInfo &getFileInfo();

  void addEvent(std::unique_ptr<SeismEvent>);
  bool removeEvent(const QUuid &);
  int getEventsNumber() const;
  const std::map<QUuid, std::unique_ptr<SeismEvent>> &getEventsMap() const;
  const std::unique_ptr<Data::SeismEvent> &getEvent(const QUuid &) const;

  void addHorizon(std::unique_ptr<SeismHorizon>);
  bool removeHorizon(const QUuid &);
  int getHorizonsNumber() const;
  const std::map<QUuid, std::unique_ptr<SeismHorizon>> &getHorizonsMap() const;
  const std::unique_ptr<Data::SeismHorizon> &getHorizon(const QUuid &) const;

signals:
  void addedEvent(const std::unique_ptr<Data::SeismEvent> &) const;
  void removedEvent(const QUuid &) const;

  void addedHorizon(const std::unique_ptr<Data::SeismHorizon> &) const;
  void removedHorizon(const QUuid &) const;

private:
  static const QUuid generateUuid();

  bool _isSaved{false};

  QString _name;
  QDateTime _dateTime;
  QFileInfo _fileInfo;

  std::map<QUuid, std::unique_ptr<SeismEvent>> _events_map;
  std::map<QUuid, std::unique_ptr<SeismHorizon>> _horizons_map;
};

} // namespace Data
