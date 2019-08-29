#include "seismproject.h"

#include <QDir>
#include <QJsonArray>

namespace Data {
SeismProject::SeismProject(QObject *parent) : QObject(parent) {
  // NOTE: hard-code insert receivers
  for (int i = 0; i < 8; ++i) {
    std::unique_ptr<SeismReceiver> receiver = std::make_unique<SeismReceiver>();
    receiver->setName(QString::number(i + 1));
    //    receiver->setChannelNum(3);
    receiver->setUuid(generateUuid());

    _receivers.push_back(std::move(receiver));
  }
  // end...

  // NOTE: hard-code insert wells
  for (int i = 0; i < 2; ++i) {
    std::unique_ptr<SeismWell> well = std::make_unique<SeismWell>();
    well->setName(QString::number(i + 1));

    auto uuid = generateUuid();
    well->setUuid(uuid);
    _wells_map[uuid] = std::move(well);
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

  if (json.contains("Wells")) {
    QJsonArray wellsArray(json["Wells"].toArray());
    int idx = 0;
    for (auto objWell : wellsArray) {
      try {
        auto seismWell =
            std::make_unique<SeismWell>(objWell.toObject(), fileInfo.dir());
        auto uuid = seismWell->getUuid();
        _wells_map[uuid] = std::move(seismWell);
      } catch (std::runtime_error &err) {
        err_msg += "Wells (idx: " + std::to_string(idx) + ")\n";
        err_msg += err.what();
      }
      ++idx;
    }
  } else {
    err_msg += "::Wells : not found\n";
  }

  if (json.contains("Receivers")) {
    QJsonArray receiversArray(json["Receivers"].toArray());
    int idx = 0;
    for (auto objReceiver : receiversArray) {
      try {
        auto seismReceiver =
            std::make_unique<SeismReceiver>(objReceiver.toObject());
        auto uuid = generateUuid();
        seismReceiver->setUuid(uuid);
        //        _receivers_map[uuid] = std::move(seismReceiver);
        _receivers.push_back(std::move(seismReceiver));
      } catch (std::runtime_error &err) {
        err_msg += "Receivers (idx: " + std::to_string(idx) + ")\n";
        err_msg += err.what();
      }
      ++idx;
    }
  } else {
    err_msg += "::Receivers : not found\n";
  }

  if (json.contains("Events")) {
    QJsonArray eventsArray(json["Events"].toArray());
    int idx = 0;
    for (auto objEvent : eventsArray) {
      try {
        auto seismEvent = std::make_unique<SeismEvent>(
            objEvent.toObject(), _receivers, fileInfo.dir());
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

  dataDir = _fileInfo.dir();
  if (dataDir.cd(SeismWell::_default_path)) {
    dataDir.removeRecursively();
    dataDir = _fileInfo.dir();
  }
  dataDir.mkpath(SeismWell::_default_path);

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

  QJsonArray receiversArray;
  QJsonObject receiverObj;
  for (auto &receiver : _receivers) {
    receiversArray.append(receiver->writeToJson(receiverObj));
  }
  json["Receivers"] = receiversArray;

  QJsonArray wellsArray;
  QJsonObject wellObj;
  for (auto &itr : _wells_map) {
    wellsArray.append((itr.second)->writeToJson(wellObj, _fileInfo.dir()));
  }
  json["Wells"] = wellsArray;

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

// Well template`s
template <> void SeismProject::add<SeismWell>(std::unique_ptr<SeismWell> well) {
  _isSaved = false;
  auto uuid = generateUuid();
  well->setUuid(uuid);
  _wells_map[uuid] = std::move(well);

  emit addedWell(_wells_map[uuid]);
}

template <> bool SeismProject::remove<SeismWell>(const QUuid &uuid) {
  if (_wells_map.erase(uuid)) {
    _isSaved = false;
    emit removedWell(uuid);
    return true;
  }
  return false;
}

template <> int SeismProject::getNumber<SeismWell>() const {
  return static_cast<int>(_wells_map.size());
}

template <>
const std::unique_ptr<SeismWell> &
SeismProject::get<SeismWell>(const QUuid &uuid) const {
  return _wells_map.at(uuid);
}

template <>
const std::map<QUuid, std::unique_ptr<SeismWell>> &
SeismProject::getAllMap<SeismWell>() const {
  return _wells_map;
}
// end of Well template`s

// Receiver template`s
template <>
void SeismProject::add<SeismReceiver>(std::unique_ptr<SeismReceiver> receiver) {
  _isSaved = false;
  auto uuid = generateUuid();
  receiver->setUuid(uuid);
  _receivers.push_back(std::move(receiver));

  emit addedReceiver(_receivers.back());
}

template <> bool SeismProject::remove<SeismReceiver>(const QUuid &uuid) {
  for (auto itr = _receivers.begin(); itr != _receivers.end(); ++itr) {
    if (uuid == (*itr)->getUuid()) {
      _receivers.erase(itr);
      emit removedReceiver(uuid);
      return true;
    }
  }
  return false;
}

template <> int SeismProject::getNumber<SeismReceiver>() const {
  return static_cast<int>(_receivers.size());
}

template <>
const std::list<std::unique_ptr<SeismReceiver>> &
SeismProject::getAllList<SeismReceiver>() const {
  return _receivers;
}
// end of Receiver template`s

} // namespace Data
