#include "seismproject.h"

#include <QDir>
#include <QJsonArray>

namespace Data {
SeismProject::SeismProject(QObject *parent) : QObject(parent) {}

SeismProject::SeismProject(const QJsonObject &json, const QFileInfo &fileInfo,
                           QObject *parent)
    : QObject(parent), _fileInfo(fileInfo) {

  std::string err_msg;

  if (json.contains("name")) {
    _name = json["name"].toString();
  } else {
    err_msg += "::name : not found\n";
  }

  if (json.contains("date")) {
    _dateTime =
        QDateTime::fromString(json["date"].toString(), "dd.MM.yy hh:mm:ss");
  } else {
    err_msg += "::date : not found\n";
  }

  if (json.contains("Events")) {
    QJsonArray eventsArray(json["Events"].toArray());
    int idx = 0;
    for (auto objEvent : eventsArray) {
      try {
        auto seismEvent =
            std::make_unique<SeismEvent>(objEvent.toObject(), fileInfo.dir());
        auto uuid = seismEvent->getUuid();
        _events_map[uuid] = std::move(seismEvent);
      } catch (std::runtime_error &err) {
        err_msg += "Events (idx: " + std::to_string(idx) + ")\n";
        err_msg += err.what();
      }
      ++idx;
    }
  } else {
    err_msg += "::Horizons : not found\n";
  }

  if (json.contains("Horizons")) {
    QJsonArray horizonssArray(json["Horizons"].toArray());
    int idx = 0;
    for (auto objHorizon : horizonssArray) {
      try {
        auto seismHorizon = std::make_unique<SeismHorizon>(
            objHorizon.toObject(), fileInfo.dir());
        auto uuid = seismHorizon->getUuid();
        _horizons_map[uuid] = std::move(seismHorizon);
      } catch (std::runtime_error &err) {
        err_msg += "Horizons (idx: " + std::to_string(idx) + ")\n";
        err_msg += err.what();
      }
      ++idx;
    }
  } else {
    err_msg += "::Horizons : not found\n";
  }

  _isSaved = true;

  if (!err_msg.empty()) {
    throw std::runtime_error(err_msg);
  }
}

bool SeismProject::exist() const { return _fileInfo.isFile(); }

bool SeismProject::isSaved() const { return _isSaved; }

QJsonObject &SeismProject::writeToJson(QJsonObject &json,
                                       const QFileInfo &fileInfo) {
  assert(fileInfo.isFile());

  _fileInfo = fileInfo;

  json["name"] = _name;
  json["date"] = _dateTime.toString("dd.MM.yy hh:mm:ss");

  QDir dataDir = _fileInfo.dir();
  if (dataDir.cd(SeismEvent::_default_path)) {
    dataDir.removeRecursively();
    dataDir = _fileInfo.dir();
  }
  dataDir.mkpath(SeismEvent::_default_path);

  dataDir = _fileInfo.dir();
  if (dataDir.cd(SeismHorizon::_default_path)) {
    dataDir.removeRecursively();
    dataDir = _fileInfo.dir();
  }
  dataDir.mkpath(SeismHorizon::_default_path);

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

bool SeismProject::removeEvent(const QUuid &uuid) {
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

const std::map<QUuid, std::unique_ptr<SeismEvent>> &
SeismProject::getEventsMap() const {
  return _events_map;
}

const std::unique_ptr<SeismEvent> &
SeismProject::getEvent(const QUuid &uuid) const {
  return _events_map.at(uuid);
}

void SeismProject::addHorizon(std::unique_ptr<SeismHorizon> horizon) {
  _isSaved = false;

  auto uuid = generateUuid();
  horizon->setUuid(uuid);
  _horizons_map[uuid] = std::move(horizon);

  emit addedHorizon(_horizons_map[uuid]);
}

bool SeismProject::removeHorizon(const QUuid &uuid) {
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

const std::map<QUuid, std::unique_ptr<SeismHorizon>> &
SeismProject::getHorizonsMap() const {
  return _horizons_map;
}

const std::unique_ptr<SeismHorizon> &
SeismProject::getHorizon(const QUuid &uuid) const {
  return _horizons_map.at(uuid);
}

const QUuid SeismProject::generateUuid() { return QUuid::createUuid(); }

} // namespace Data
