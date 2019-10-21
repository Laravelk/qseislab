#pragma once

#include "seismevent.h"
#include "seismhorizon.h"
#include "seismreceiver.h"
#include "seismwell.h"

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

  template <typename T> void add(std::unique_ptr<T>);
  template <typename T> void update(std::unique_ptr<T>);
  template <typename T> bool remove(const QUuid &);
  template <typename T> int getNumber() const;
  template <typename T> const std::unique_ptr<T> &get(const QUuid &) const;

  template <typename T>
  const std::map<QUuid, std::unique_ptr<T>> &getAllMap() const;
  template <typename T> void setAllMap(std::map<QUuid, std::unique_ptr<T>> &);

  void processEvents();

signals:
  void addedEvent(const std::unique_ptr<Data::SeismEvent> &) const;
  void updatedEvent(const std::unique_ptr<Data::SeismEvent> &) const;
  void removedEvent(const QUuid &) const;
  void processedEvents() const;

  void addedHorizon(const std::unique_ptr<Data::SeismHorizon> &) const;
  void removedHorizon(const QUuid &) const;

  void addedWell(const std::unique_ptr<Data::SeismWell> &) const;
  void removedWell(const QUuid &) const;
  void removedReceiver(const QUuid &) const;

private:
  bool _isSaved{false};

  QString _name;
  QDateTime _dateTime;
  QFileInfo _fileInfo;

  std::map<QUuid, std::unique_ptr<SeismEvent>> _events_map;
  std::map<QUuid, std::unique_ptr<SeismHorizon>> _horizons_map;
  std::map<QUuid, std::unique_ptr<SeismWell>> _wells_map;
};

} // namespace Data
