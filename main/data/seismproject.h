#pragma once

#include "projectsettings.h"
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

  ProjectSettings *getSettings();
  ProjectSettings const *getSettings() const;

  template <typename T> void add(const std::shared_ptr<T> &);
  template <typename T> void update(const std::shared_ptr<T> &);
  template <typename T> bool remove(const QUuid &);
  template <typename T> int getAmount() const;
  template <typename T> const std::shared_ptr<T> &get(const QUuid &) const;

  template <typename T>
  const std::map<QUuid, std::shared_ptr<T>> &getAllMap() const;
  template <typename T> void setAllMap(std::map<QUuid, std::shared_ptr<T>> &);
  template <typename T>
  void updateMap(std::map<QUuid, std::shared_ptr<T>> &); // NOTE: ok-?

  template <typename T> const std::list<std::shared_ptr<T>> &getAll() const;
  template <typename T> void setAll(const std::list<std::shared_ptr<T>> &);

  void processEvents();

  //  bool addReceiver(const QUuid &, const std::shared_ptr<Data::SeismReceiver>
  //  &); void removeAllReceivers();

signals:
  void addedEvent(const std::shared_ptr<Data::SeismEvent> &) const;
  void updatedEvent(const std::shared_ptr<Data::SeismEvent> &) const;
  void removedEvent(const QUuid &) const;
  void processedEvents() const;

  void addedHorizon(const std::shared_ptr<Data::SeismHorizon> &) const;
  void removedHorizon(const QUuid &) const;

  void addedWell(const std::shared_ptr<Data::SeismWell> &) const;
  void removedWell(const QUuid &) const;

  void addedReceiver(
      const std::shared_ptr<Data::SeismReceiver> &) const; // TODO: re-see
  void removedReceiver(const QUuid &) const;               // TODO: re-see

private:
  bool _isSaved{false};

  QString _name;
  QDateTime _dateTime;
  QFileInfo _fileInfo;

  std::shared_ptr<ProjectSettings> _settings;

  std::map<QUuid, std::shared_ptr<SeismEvent>> _events_map;

  std::map<QUuid, std::shared_ptr<SeismHorizon>> _horizons_map;
  std::map<QUuid, std::shared_ptr<SeismWell>> _wells_map;

  //  std::map<QUuid, std::shared_ptr<SeismReceiver>> _receivers_map;
  std::list<std::shared_ptr<SeismReceiver>> _receivers;
};

} // namespace Data
