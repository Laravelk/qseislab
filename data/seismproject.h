#pragma once

#include "seismevent.h"
#include "seismhorizon.h"
#include "seismreciever.h"

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
  template <typename T> bool remove(const QUuid &);
  template <typename T> int getNumber() const;
  template <typename T> const std::unique_ptr<T> &get(const QUuid &) const;

  // TODO: вопрос про контейнеры
  // TODO: re-name
  template <typename T>
  const std::map<QUuid, std::unique_ptr<T>> &getAllMap() const;
  template <typename T> const std::list<std::unique_ptr<T>> &getAllList() const;

  void processEvents();

  //  void addEvent(std::unique_ptr<SeismEvent>);
  //  bool removeEvent(const QUuid &);
  //  int getEventsNumber() const;
  //  const std::map<QUuid, std::unique_ptr<SeismEvent>> &getEventsMap() const;
  //  const std::unique_ptr<Data::SeismEvent> &getEvent(const QUuid &) const;

  //  void addHorizon(std::unique_ptr<SeismHorizon>);
  //  bool removeHorizon(const QUuid &);
  //  int getHorizonsNumber() const;
  //  const std::map<QUuid, std::unique_ptr<SeismHorizon>> &getHorizonsMap()
  //  const; const std::unique_ptr<Data::SeismHorizon> &getHorizon(const QUuid
  //  &) const;

  //  void addReciever(std::unique_ptr<SeismReciever>);
  //  bool removeReciever(const QUuid &);
  //  int getRecieversNumber() const;
  //  const std::list<std::unique_ptr<SeismReciever>> &getRecievers() const;

signals:
  void addedEvent(const std::unique_ptr<Data::SeismEvent> &) const;
  void removedEvent(const QUuid &) const;
  void updateEvents() const;

  void addedHorizon(const std::unique_ptr<Data::SeismHorizon> &) const;
  void removedHorizon(const QUuid &) const;

  void addedReciever(const std::unique_ptr<Data::SeismReciever> &) const;
  void removedReciever(const QUuid &) const;

private:
  static const QUuid generateUuid();

  bool _isSaved{false};

  QString _name;
  QDateTime _dateTime;
  QFileInfo _fileInfo;

  std::map<QUuid, std::unique_ptr<SeismEvent>> _events_map;
  std::map<QUuid, std::unique_ptr<SeismHorizon>> _horizons_map;
  //  std::map<QUuid, std::unique_ptr<SeismReciever>> _recievers_map;

  std::list<std::unique_ptr<SeismReciever>> _recievers;
};

} // namespace Data

//#include "seismproject_impl.h"
