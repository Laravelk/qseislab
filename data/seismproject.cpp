#include "seismproject.h"

#include <QDir>
#include <QJsonArray>

#include <assert.h>
#include <iostream> // TODO: remove

namespace Data {
SeismProject::SeismProject(QObject *parent)
    : QObject(parent), _settings(std::make_shared<ProjectSettings>()) {
  // NOTE: hard-code insert events
  //    for (int i = 0; i < 4; ++i) {
  //      std::unique_ptr<SeismEvent> event = std::make_unique<SeismEvent>();

  //      event->setDateTime(
  //          QDateTime::currentDateTime().addDays(i).addSecs(120 * i));
  //      event->setType(i);

  //      _events_map[event->getUuid()] = std::move(event);
  //    }
  //     end...

  //   NOTE: hard-code insert wells
  std::shared_ptr<SeismWell> well = std::make_shared<SeismWell>();
  well->setName("Mon_TOOLS_233");
  well->addPoint(Point(0, 0, 0));
  auto uuid = well->getUuid();
  for (int j = 0; j < 4; ++j) {
    auto receiver = std::make_shared<Data::SeismReceiver>(well.get());
    for (int i = 0; i < 3; ++i) {
      receiver->addChannel(std::make_shared<Data::SeismChannelReceiver>());
    }
    //    well->addReceiver(receiver);
    _receivers.push_back(receiver);
  }
  _wells_map[uuid] = std::move(well);

  well = std::make_shared<SeismWell>();
  well->setName("Mon_TOOLS_244");
  well->addPoint(Point(0, 0, 0));
  uuid = well->getUuid();
  for (int j = 0; j < 8; ++j) {
    auto receiver = std::make_shared<Data::SeismReceiver>(well.get());
    for (int i = 0; i < 3; ++i) {
      receiver->addChannel(std::make_shared<Data::SeismChannelReceiver>());
    }
    //    well->addReceiver(receiver);
    _receivers.push_back(receiver);
  }
  _wells_map[uuid] = std::move(well);
  // end...
}

SeismProject::SeismProject(const QJsonObject &json, const QFileInfo &fileInfo,
                           QObject *parent)
    : QObject(parent), _fileInfo(fileInfo),
      _settings(std::make_shared<ProjectSettings>()) {

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
        auto seismHorizon = std::make_shared<SeismHorizon>(
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
            std::make_shared<SeismWell>(objWell.toObject(), fileInfo.dir());
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
        auto obj = objReceiver.toObject();
        if (obj.contains("wellUuid")) {
          auto wellUuid = QUuid::fromString(obj["wellUuid"].toString());
          auto itr_well = _wells_map.find(wellUuid);
          if (wellUuid.isNull() || _wells_map.end() != itr_well) {
            auto seismReceiver =
                std::make_shared<SeismReceiver>(obj, itr_well->second.get());
            //            auto uuid = seismReceiver->getUuid();
            //            _receivers_map[uuid] = seismReceiver;
            _receivers.push_back(seismReceiver);
          } else {
            throw std::runtime_error(
                "::wellUuid : there isn`t well with the specified uuid\n");
          }
        } else {
          throw std::runtime_error("::wellUuid : not found\n");
        }

      } catch (std::runtime_error &err) {
        err_msg += "Receivers (idx: " + std::to_string(idx) + ")\n";
        err_msg += err.what();
      }
      ++idx;
    }
  } else {
    err_msg += "::Wells : not found\n";
  }

  if (json.contains("Events")) {
    QJsonArray eventsArray(json["Events"].toArray());
    int idx = 0;
    for (auto objEvent : eventsArray) {
      try {
        auto seismEvent = std::make_shared<SeismEvent>(
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
    err_msg += "::Events : not found\n";
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
  for (auto &uuid_event : _events_map) {
    eventsArray.append(
        (uuid_event.second)->writeToJson(eventObj, _fileInfo.dir()));
  }
  json["Events"] = eventsArray;

  QJsonArray horizonsArray;
  QJsonObject horizonObj;
  for (auto &uuid_horizon : _horizons_map) {
    horizonsArray.append(
        (uuid_horizon.second)->writeToJson(horizonObj, _fileInfo.dir()));
  }
  json["Horizons"] = horizonsArray;

  QJsonArray wellsArray;
  QJsonObject wellObj;
  for (auto &uuid_well : _wells_map) {
    wellsArray.append(
        (uuid_well.second)->writeToJson(wellObj, _fileInfo.dir()));
  }
  json["Wells"] = wellsArray;

  QJsonArray receiversArray;
  QJsonObject receiverObj;
  for (auto &receiver : _receivers) {
    receiversArray.append(receiver->writeToJson(receiverObj));
  }
  json["Receivers"] = receiversArray;

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

// void SeismProject::setSettings(const ProjectSettings &settings) {
//  _setting = settings;
//}

ProjectSettings *SeismProject::getSettings() { return _settings.get(); }

ProjectSettings const *SeismProject::getSettings() const {
  return _settings.get();
}

void SeismProject::processEvents() {
  for (auto &itr : _events_map) {
    (itr.second)->process();
  }
  emit processedEvents();
}

//// Receivers func`s
// bool SeismProject::addReceiver(const QUuid &wellUuid,
//                               const std::shared_ptr<SeismReceiver> &receiver)
//                               {
//  for (auto &uuid_well : _wells_map) {
//    if (wellUuid == uuid_well.first) {
//      auto &well = uuid_well.second;
//      well->addReceiver(receiver);
//      emit addedReceiver(receiver);
//      return true;
//    }
//  }
//  return false;
//}

// void SeismProject::removeAllReceivers() {
//  for (auto &uuid_well : _wells_map) {
//    for (auto &receiver : uuid_well.second->getReceivers()) {
//      emit removedReceiver(receiver->getUuid());
//    }
//    uuid_well.second->removeAllReceivers();
//  }
//}
//// end of Receivers func`s

// Event template`s
template <>
void SeismProject::add<SeismEvent>(const std::shared_ptr<SeismEvent> &event) {
  _isSaved = false;

  connect(event.get(), &SeismEvent::infoChanged,
          [this, &event] { emit updatedEvent(event); });
  connect(event.get(), &SeismEvent::dataChanged,
          [this, &event] { emit updatedEvent(event); });

  auto &uuid = event->getUuid();
  _events_map[uuid] = event;

  // TODO: сделать коннект с ивентов, чтоюы при любом его апдейте мы знали и
  // говорили остальным

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

// template <>
// void SeismProject::update<SeismEvent>(std::unique_ptr<SeismEvent> event) {
//  _isSaved = false;
//  auto uuid = event->getUuid();
//  _events_map[uuid] = std::move(event);

//  emit updatedEvent(_events_map[uuid]);
//}

template <> int SeismProject::getAmount<SeismEvent>() const {
  return static_cast<int>(_events_map.size());
}

template <>
const std::shared_ptr<SeismEvent> &
SeismProject::get<SeismEvent>(const QUuid &uuid) const {
  return _events_map.at(uuid);
}

template <>
const std::map<QUuid, std::shared_ptr<SeismEvent>> &
SeismProject::getAllMap<SeismEvent>() const {
  return _events_map;
}
// end of Event template`s

// Horizon template`s
template <>
void SeismProject::add<SeismHorizon>(
    const std::shared_ptr<SeismHorizon> &horizon) {
  _isSaved = false;
  //  auto uuid = generateUuid();
  //  horizon->setUuid(uuid);
  auto uuid = horizon->getUuid();
  _horizons_map[uuid] = horizon;

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

template <> int SeismProject::getAmount<SeismHorizon>() const {
  return static_cast<int>(_horizons_map.size());
}

// template <>
// const std::shared_ptr<SeismHorizon> &
// SeismProject::get<SeismHorizon>(const QUuid &uuid) const {
//  return _horizons_map.at(uuid);
//}

template <>
const std::map<QUuid, std::shared_ptr<SeismHorizon>> &
SeismProject::getAllMap<SeismHorizon>() const {
  return _horizons_map;
}

template <>
void SeismProject::setAllMap<SeismHorizon>(
    std::map<QUuid, std::shared_ptr<SeismHorizon>> &horizons_map) {
  for (auto &uuid_horizon : _horizons_map) {
    emit removedHorizon(uuid_horizon.first);
  }

  _horizons_map = horizons_map;

  for (auto &uuid_horizon : _horizons_map) {
    emit addedHorizon(uuid_horizon.second);
  }
}

template <>
void SeismProject::updateMap<SeismHorizon>(
    std::map<QUuid, std::shared_ptr<SeismHorizon>> &horizons_map) {

  //    for(auto& uuid_horizon : _horizons_map) {
  //        if(horizons_map.find)
  //    }

  //    for (auto &uuid_horizon : _horizons_map) {
  //        emit removedHorizon(uuid_horizon.first);
  //    }

  //    _horizons_map = horizons_map;

  //    for (auto &uuid_horizon : _horizons_map) {
  //        emit addedHorizon(uuid_horizon.second);
  //    }
}
// end of Horizon template`s

// Well template`s
template <>
void SeismProject::add<SeismWell>(const std::shared_ptr<SeismWell> &well) {
  _isSaved = false;
  //  auto uuid = generateUuid();
  //  well->setUuid(uuid);
  auto uuid = well->getUuid();
  _wells_map[uuid] = well;

  emit addedWell(_wells_map[uuid]);
}

template <> bool SeismProject::remove<SeismWell>(const QUuid &uuid) {
  std::set<QUuid> uuids_receiver_removing;
  //  for (auto &uuid_receiver : _receivers_map) {
  //    if (uuid == uuid_receiver.second->getSourseWell()->getUuid()) {
  //      uuids_receiver_removing.insert(uuid_receiver.first);
  //    }
  //  }
  for (auto &receiver : _receivers) {
    if (uuid == receiver->getSourseWell()->getUuid()) {
      uuids_receiver_removing.insert(receiver->getUuid());
    }
  }

  // NOTE: bad way!
  for (auto &removing_uuid : uuids_receiver_removing) {
    for (auto itr = _receivers.begin(); itr != _receivers.end(); ++itr) {
      if ((*itr)->getUuid() == removing_uuid) {
        _receivers.erase(itr);
        break;
      }
    }
    //    _receivers_map.erase(removing_uuid);
    emit removedReceiver(removing_uuid);
  }

  if (_wells_map.erase(uuid)) {
    _isSaved = false;
    emit removedWell(uuid);
    return true;
  }
  return false;

  //  if (_wells_map.end() != _wells_map.find(uuid)) {
  //    _isSaved = false;
  //    for (auto &receiver : _wells_map[uuid]->getReceivers()) {
  //      emit removedReceiver(receiver->getUuid());
  //    }
  //    _wells_map.erase(uuid);
  //    emit removedWell(uuid);
  //    return true;
  //  }
  //  return false;
}

template <> int SeismProject::getAmount<SeismWell>() const {
  return static_cast<int>(_wells_map.size());
}

// template <>
// const std::shared_ptr<SeismWell> &
// SeismProject::get<SeismWell>(const QUuid &uuid) const {
//  return _wells_map.at(uuid);
//}

template <>
const std::map<QUuid, std::shared_ptr<SeismWell>> &
SeismProject::getAllMap<SeismWell>() const {
  return _wells_map;
}

template <>
void SeismProject::setAllMap<SeismWell>(
    std::map<QUuid, std::shared_ptr<SeismWell>> &wells_map) {
  for (auto &uuid_well : _wells_map) {
    //    emit removedWell(uuid_well.first);
    remove<SeismWell>(uuid_well.first);
  }

  _wells_map = wells_map;

  for (auto &uuid_well : _wells_map) {
    add<SeismWell>(uuid_well.second);
    //    emit addedWell(uuid_well.second);
  }

  //  for (auto &uuid_well : _wells_map) {
  //    auto &uuid = uuid_well.first;
  //    if (wells.end() == wells.find(uuid)) {
  //      for (auto &receiver : uuid_well.second->getReceivers()) {
  //        emit removedReceiver(receiver->getUuid());
  //      }
  //      _wells_map.erase(uuid);
  //      emit removedWell(uuid);
  //    }
  //  }

  //  for (auto &uuid_well : wells) {
  //    auto &uuid = uuid_well.first;
  //    if (_wells_map.end() == _wells_map.find(uuid)) {
  //      for (auto &receiver : uuid_well.second->getReceivers()) {
  //        emit addedReceiver(receiver);
  //      }
  //      _wells_map[uuid] = wells[uuid];
  //      emit addedWell(_wells_map[uuid]);
  //    }
  //  }
}
// end of Well template`s

// Receiver template`s
template <>
void SeismProject::add<SeismReceiver>(
    const std::shared_ptr<SeismReceiver> &receiver) {
  _isSaved = false;

  //  auto &uuid = receiver->getUuid();
  //  _receivers_map[uuid] = receiver;
  _receivers.push_back(receiver);

  _settings->getRotateDataParameters().setReceivers(
      _receivers); // TODO: переделать!!!

  emit addedReceiver(receiver);

  //    connect(event.get(), &SeismEvent::infoChanged,
  //            [this, &event] { emit updatedEvent(event); });
  //    connect(event.get(), &SeismEvent::dataChanged,
  //            [this, &event] { emit updatedEvent(event); });

  //    auto &uuid = event->getUuid();
  //    _events_map[uuid] = event;

  //    // TODO: сделать коннект с ивентов, чтоюы при любом его апдейте мы знали
  //    и
  //    // говорили остальным

  //    emit addedEvent(_events_map[uuid]);
}

template <> bool SeismProject::remove<SeismReceiver>(const QUuid &uuid) {
  for (auto itr = _receivers.begin(); itr != _receivers.end(); ++itr) {
    if (uuid == (*itr)->getUuid()) {
      _receivers.erase(itr);
      emit removedReceiver(uuid);
      return true;
    }
  }

  _settings->getRotateDataParameters().setReceivers(
      _receivers); // TODO: переделать!!!

  //  if (_receivers_map.erase(uuid)) {
  //    _isSaved = false;
  //    emit removedReceiver(uuid);
  //    return true;
  //  }
  return false;
}

// template <>
// void SeismProject::update<SeismEvent>(std::unique_ptr<SeismEvent> event) {
//  _isSaved = false;
//  auto uuid = event->getUuid();
//  _events_map[uuid] = std::move(event);

//  emit updatedEvent(_events_map[uuid]);
//}

template <> int SeismProject::getAmount<SeismReceiver>() const {
  //  return static_cast<int>(_receivers_map.size());
  return static_cast<int>(_receivers.size());
}

// template <>
// const std::shared_ptr<SeismReceiver> &
// SeismProject::get<SeismReceiver>(const QUuid &uuid) const {
//  return _receivers_map.at(uuid);
//}

// template <>
// const std::map<QUuid, std::shared_ptr<SeismReceiver>> &
// SeismProject::getAllMap<SeismReceiver>() const {
//  return _receivers_map;
//}

template <>
const std::list<std::shared_ptr<SeismReceiver>> &
SeismProject::getAll<SeismReceiver>() const {
  return _receivers;
}

template <>
void SeismProject::setAll<SeismReceiver>(
    const std::list<std::shared_ptr<SeismReceiver>> &receivers) {
  //  return _receivers;
  for (auto &receiver : _receivers) {
    emit removedReceiver(receiver->getUuid());
  }
  _receivers = receivers;

  for (auto &receiver : _receivers) {
    emit addedReceiver(receiver);
  }

  _settings->getRotateDataParameters().setReceivers(
      _receivers); // TODO: переделать!!!
}

// end of Receiver template`s

} // namespace Data
