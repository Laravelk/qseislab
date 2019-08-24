#pragma once

//#include "seismproject.h"

namespace Data {

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
