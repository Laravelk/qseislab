#include "seismproject.h"

#include <QDir>
#include <QJsonArray>

namespace Data {
SeismProject::SeismProject(QObject *parent) : QObject(parent) {}

SeismProject::SeismProject(const QJsonObject &json, const QFileInfo &fileInfo,
                           QObject *parent)
    : QObject(parent), _fileInfo(fileInfo) {
  if (!(json.contains("Events") && json.contains("Horizons") &&
        json.contains("name") && json.contains("date"))) {
    throw std::runtime_error("Not found json-field (SeismProject)");
  }

  _name = json["name"].toString();
  _dateTime =
      QDateTime::fromString(json["date"].toString(), "dd.MM.yy hh:mm:ss");

  QJsonArray eventsArray(json["Events"].toArray());
  unsigned num = 0;
  for (auto objEvent : eventsArray) {
    auto seismEvent =
        std::make_unique<SeismEvent>(objEvent.toObject(), fileInfo.dir());
    //        auto uuid = generateUuid();
    //        seismEvent->setUuid(uuid);
    auto uuid = seismEvent->getUuid();
    _events_map[uuid] = std::move(seismEvent);
  }

  QJsonArray horizonssArray(json["Horizons"].toArray());
  num = 0;
  for (auto objHorizon : horizonssArray) {
    auto seismHorizon =
        std::make_unique<SeismHorizon>(objHorizon.toObject(), fileInfo.dir());
    //        auto uuid = generateUuid();
    //        seismHorizon->setUuid(uuid);
    auto uuid = seismHorizon->getUuid();
    _horizons_map[uuid] = std::move(seismHorizon);
  }

  _isSaved = true;
}

bool SeismProject::exist() const {
  // TODO: it`s OK?
  return _fileInfo.isFile();
}

bool SeismProject::isSaved() const { return _isSaved; }

QJsonObject &SeismProject::writeToJson(QJsonObject &json,
                                       const QFileInfo &fileInfo) {
  assert(fileInfo.isFile());

  _fileInfo = fileInfo;

  json["name"] = _name;
  json["date"] = _dateTime.toString("dd.MM.yy hh:mm:ss");

  // TODO: нужно ли удалять старые, если сохраняем в директорию, где был другой
  // проект ? NOTE: тут ли должны создаваться директории для данных?
  QDir dataDir = _fileInfo.dir();
  dataDir.mkpath(SeismEvent::_default_path);
  dataDir.cd(SeismEvent::_default_path);

  dataDir = _fileInfo.dir();
  dataDir.mkpath(SeismHorizon::_default_path);
  dataDir.cd(SeismHorizon::_default_path);

  QJsonArray eventsArray;
  QJsonObject eventObj;
  unsigned num = 0;
  for (auto &itr : _events_map) {
    eventsArray.append((itr.second)->writeToJson(eventObj, _fileInfo.dir()));
  }
  json["Events"] = eventsArray;

  QJsonArray horizonsArray;
  QJsonObject horizonObj;
  num = 0;
  for (auto &itr : _horizons_map) {
    horizonsArray.append(
        (itr.second)->writeToJson(horizonObj, _fileInfo.dir()));
  }
  json["Horizons"] = horizonsArray;

  _isSaved = true;

  return json;
}

void SeismProject::setName(const QString &name) {
  if (name != _name) {
    _isSaved = false;
    _name = name;
  }
}

const QString &SeismProject::getName() const { return _name; }

void SeismProject::setDate(const QDate &date) {
  if (date != _dateTime.date()) {
    _isSaved = false;
    _dateTime.setDate(date);
  }
}

void SeismProject::setTime(const QTime &time) {
  if (time != _dateTime.time()) {
    _isSaved = false;
    _dateTime.setTime(time);
  }
}

void SeismProject::setDateTime(const QDateTime &dateTime) {
  if (dateTime != _dateTime) {
    _isSaved = false;
    _dateTime = dateTime;
  }
}

const QDateTime &SeismProject::getDateTime() const { return _dateTime; }

void SeismProject::setFileInfo(const QFileInfo &fileInfo) {
  if (fileInfo != _fileInfo) {
    _isSaved = false;
    _fileInfo = fileInfo;
  }
}

const QFileInfo &SeismProject::getFileInfo() { return _fileInfo; }

void SeismProject::addEvent(std::unique_ptr<SeismEvent> event) {
  _isSaved = false;

  auto uuid = generateUuid();
  event->setUuid(uuid);
  _events_map[uuid] = std::move(event);

  emit addedEvent(_events_map[uuid]);
}

bool SeismProject::removeEvent(const SeismEvent::Uuid &uuid) {
  // NOTE: преобразование к bool корректное ?
  if (_events_map.erase(uuid)) {
    _isSaved = false;
    emit removedEvent(uuid);
    return true;
  }
  return false;
}

int SeismProject::getEventsNumber() const {
  return static_cast<int>(_events_map.size());
}

const std::map<Data::SeismEvent::Uuid, std::unique_ptr<SeismEvent>> &
SeismProject::getEventsMap() const {
  return _events_map;
}

const std::unique_ptr<SeismEvent> &
SeismProject::getEvent(const SeismEvent::Uuid &uuid) const {
  return _events_map.at(uuid);
}

void SeismProject::addHorizon(std::unique_ptr<SeismHorizon> horizon) {
  _isSaved = false;

  auto uuid = generateUuid();
  horizon->setUuid(uuid);
  _horizons_map[uuid] = std::move(horizon);

  emit addedHorizon(_horizons_map[uuid]);
}

bool SeismProject::removeHorizon(const SeismHorizon::Uuid &uuid) {
  // NOTE: преобразование к bool корректное ?
  if (_horizons_map.erase(uuid)) {
    _isSaved = false;
    emit removedHorizon(uuid);
    return true;
  }
  return false;
}

int SeismProject::getHorizonsNumber() const {
  return static_cast<int>(_horizons_map.size());
}

const std::map<SeismHorizon::Uuid, std::unique_ptr<SeismHorizon>> &
SeismProject::getHorizonsMap() const {
  return _horizons_map;
}

const std::unique_ptr<SeismHorizon> &
SeismProject::getHorizon(const SeismHorizon::Uuid &uuid) const {
  return _horizons_map.at(uuid);
}

const QUuid SeismProject::generateUuid() { return QUuid::createUuid(); }

} // namespace Data
