#include "seismproject.h"

#include <QDir>
#include <QJsonArray>

namespace Data {
SeismProject::SeismProject(QObject *parent) : QObject(parent) {
  // NOTE: hard-code insert recievers
  for (int i = 0; i < 8; ++i) {
    std::unique_ptr<SeismReciever> reciever = std::make_unique<SeismReciever>();
    reciever->setName(QString::number(i + 1));
    reciever->setChannelNum(3);

    _recievers.push_back(std::move(reciever));
  }
  // end...
}

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
    QJsonArray horizonsArray(json["Horizons"].toArray());
    int idx = 0;
    for (auto objHorizon : horizonsArray) {
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

  if (json.contains("Recievers")) {
    QJsonArray recieversArray(json["Recievers"].toArray());
    int idx = 0;
    for (auto objReciever : recieversArray) {
      try {
        auto seismReciever =
            std::make_unique<SeismReciever>(objReciever.toObject());
        auto uuid = generateUuid();
        seismReciever->setUuid(uuid);
        //        _recievers_map[uuid] = std::move(seismReciever);
        _recievers.push_back(std::move(seismReciever));
      } catch (std::runtime_error &err) {
        err_msg += "Recievers (idx: " + std::to_string(idx) + ")\n";
        err_msg += err.what();
      }
      ++idx;
    }
  } else {
    err_msg += "::Recievers : not found\n";
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
  for (auto &itr : _events_map) {
    eventsArray.append((itr.second)->writeToJson(eventObj, _fileInfo.dir()));
  }
  json["Events"] = eventsArray;

  QJsonArray horizonsArray;
  QJsonObject horizonObj;
  for (auto &itr : _horizons_map) {
    horizonsArray.append(
        (itr.second)->writeToJson(horizonObj, _fileInfo.dir()));
  }
  json["Horizons"] = horizonsArray;

  QJsonArray recieversArray;
  QJsonObject recieverObj;
  for (auto &reciever : _recievers) {
    recieversArray.append(reciever->writeToJson(recieverObj));
  }
  json["Recievers"] = recieversArray;

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

void SeismProject::processEvents() {
  for (auto &itr : _events_map) {
    (itr.second)->process();
  }
  emit updateEvents();
}

const QUuid SeismProject::generateUuid() { return QUuid::createUuid(); }

// Event template`s
template <>
void SeismProject::add<SeismEvent>(std::unique_ptr<SeismEvent> event) {
  _isSaved = false;
  auto uuid = generateUuid();
  event->setUuid(uuid);
  _events_map[uuid] = std::move(event);

  emit addedEvent(_events_map[uuid]);
}

template <> bool SeismProject::remove<SeismEvent>(const QUuid &uuid) {
  if (_events_map.erase(uuid)) {
    _isSaved = false;
    emit removedEvent(uuid);
    return true;
  }
  return false;
}

template <> int SeismProject::getNumber<SeismEvent>() const {
  return static_cast<int>(_events_map.size());
}

template <>
const std::unique_ptr<SeismEvent> &
SeismProject::get<SeismEvent>(const QUuid &uuid) const {
  return _events_map.at(uuid);
}

template <>
const std::map<QUuid, std::unique_ptr<SeismEvent>> &
SeismProject::getAllMap<SeismEvent>() const {
  return _events_map;
}
// end of Event template`s

// Horizon template`s
template <>
void SeismProject::add<SeismHorizon>(std::unique_ptr<SeismHorizon> horizon) {
  _isSaved = false;
  auto uuid = generateUuid();
  horizon->setUuid(uuid);
  _horizons_map[uuid] = std::move(horizon);

  emit addedHorizon(_horizons_map[uuid]);
}

template <> bool SeismProject::remove<SeismHorizon>(const QUuid &uuid) {
  if (_horizons_map.erase(uuid)) {
    _isSaved = false;
    emit removedHorizon(uuid);
    return true;
  }
  return false;
}

template <> int SeismProject::getNumber<SeismHorizon>() const {
  return static_cast<int>(_horizons_map.size());
}

template <>
const std::unique_ptr<SeismHorizon> &
SeismProject::get<SeismHorizon>(const QUuid &uuid) const {
  return _horizons_map.at(uuid);
}

template <>
const std::map<QUuid, std::unique_ptr<SeismHorizon>> &
SeismProject::getAllMap<SeismHorizon>() const {
  return _horizons_map;
}
// end of Horizon template`s

// Reciever template`s
template <>
void SeismProject::add<SeismReciever>(std::unique_ptr<SeismReciever> reciever) {
  _isSaved = false;
  auto uuid = generateUuid();
  reciever->setUuid(uuid);
  _recievers.push_back(std::move(reciever));

  emit addedReciever(_recievers.back());
}

template <> bool SeismProject::remove<SeismReciever>(const QUuid &uuid) {
  for (auto itr = _recievers.begin(); itr != _recievers.end(); ++itr) {
    if (uuid == (*itr)->getUuid()) {
      _recievers.erase(itr);
      emit removedReciever(uuid);
      return true;
    }
  }
  return false;
}

template <> int SeismProject::getNumber<SeismReciever>() const {
  return static_cast<int>(_recievers.size());
}

template <>
const std::list<std::unique_ptr<SeismReciever>> &
SeismProject::getAllList<SeismReciever>() const {
  return _recievers;
}
// end of Reciever template`s

} // namespace Data
