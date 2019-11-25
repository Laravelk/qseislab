#include "seismevent.h"

#include "data/seismwell.h"
#include "io/seismcomponentreader.h"
#include "io/seismcomponentwriter.h"

#include <QJsonArray>

typedef Data::IO::SeismComponentReader SeismComponentReader;
typedef Data::IO::SeismComponentWriter SeismComponentWriter;

namespace Data {
const QString SeismEvent::_default_path = "data/events/";

SeismEvent::SeismEvent()
    : _uuid(QUuid::createUuid())
      //    , _addedDateTime(QDateTime::currentDateTime())
      ,
      _info(std::make_shared<EventInfo>()) {
  connect(_info.get(), &EventInfo::changed,
          [this] { emit infoChanged(_info.get()); });
}

SeismEvent::SeismEvent(const QJsonObject &json,
                       std::map<QUuid, std::shared_ptr<SeismWell>> &wells_map,
                       const QDir &dir)
    : _uuid(QUuid::createUuid()), _info(std::make_shared<EventInfo>()) {

  connect(_info.get(), &EventInfo::changed,
          [this] { emit infoChanged(_info.get()); });

  std::string err_msg;

  if (json.contains("type")) {
    _info->type = json["type"].toInt();
  } else {
    err_msg += "::type : not found\n";
  }

  if (json.contains("name")) {
    _info->name = json["name"].toString();
  } else {
    err_msg += "::name : not found\n";
  }

  if (json.contains("addedDateTime")) {
    _info->addedDateTime = QDateTime::fromString(
        json["addedDateTime"].toString(), "dd.MM.yy hh:mm:ss");
  } else {
    err_msg += "::addedDateTime : not found\n";
  }

  if (json.contains("stampDateTime")) {
    _info->stampDateTime = QDateTime::fromString(
        json["stampDateTime"].toString(), "dd.MM.yy hh:mm:ss:zzz");
  } else {
    err_msg += "::stampDateTime : not found\n";
  }

  if (json.contains("isProcessed")) {
    _isProcessed = json["isProcessed"].toBool();
    if (_isProcessed) {
      if (json.contains("Location")) {
        QJsonArray locationArray(json["Location"].toArray());
        if (3 == locationArray.size()) {
          _location = {locationArray[0].toDouble(), locationArray[1].toDouble(),
                       locationArray[2].toDouble()};
        } else {
          err_msg += "::Location : is not three-dimensional\n";
        }
      } else {
        err_msg += "::Location : not found\n";
      }
    }
  } else {
    err_msg += "::isProcessed : not found\n";
  }

  if (json.contains("path")) {
    _path = json["path"].toString();
    QFileInfo fileInfo(dir, _path);
    if (fileInfo.exists()) {
      _uuid = fileInfo.baseName();

      if (json.contains("Components")) {
        QJsonArray componentsArray(json["Components"].toArray());

        SeismComponentReader reader(fileInfo);
        int idx = 0;
        //        auto receivers_itr = receivers.begin();
        for (auto objComponent : componentsArray) {

          if (!reader.hasNext()) {
            err_msg += "::data : not enough data for event\n";
            break;
          }

          try {
            auto seismComponent =
                std::make_shared<SeismComponent>(objComponent.toObject());
            auto &receiverUuid = seismComponent->getReceiverUuid();
            bool findReceiver = false;
            for (auto &uuid_well : wells_map) {
              for (auto &receiver : uuid_well.second->getReceivers()) {
                if (receiver->getUuid() == receiverUuid) {
                  findReceiver = true;
                  for (int i = 0; i < receiver->getChannelAmount(); ++i) {
                    seismComponent->addTrace(reader.nextTrace());
                  }
                  addComponent(std::move(seismComponent));
                  break;
                }
              }
            }
            if (!findReceiver) {
              err_msg += "::receiver with uuid == " +
                         receiverUuid.toString().toStdString() + " not found\n";
            }
          } catch (std::runtime_error &err) {
            err_msg += "Component (idx: " + std::to_string(idx) + ")\n";
            err_msg += err.what();
          }

          ++idx;
          //          ++receivers_itr;
        }
        if (reader.hasNext()) {
          err_msg += "::data : not enough components (in json)\n";
        }
      } else {
        err_msg += "::Components : not found\n";
      }
    } else {
      err_msg += "::data-file : doesn`t exist\n";
    }
  } else {
    err_msg += "::path : not found\n";
  }

  if (!err_msg.empty()) {
    err_msg += "\n";
    throw std::runtime_error(err_msg);
  }
}

SeismEvent::SeismEvent(const SeismEvent &other)
    : _uuid(other._uuid), _path(other._path),
      //    _name(other._name),
      //      _addedDateTime(other._addedDateTime), _type(other._type),
      //      _stampDateTime(other._stampDateTime),
      _info(other._info), _isProcessed(other._isProcessed),
      _location(other._location) {

  connect(_info.get(), &EventInfo::changed,
          [this] { emit infoChanged(_info.get()); });

  for (auto &component : other._components) {
    _components.push_back(component);
  }
}

const QUuid &SeismEvent::getUuid() const { return _uuid; }

// void SeismEvent::setInfo(SeismEvent::Info info) {
//  //    _info = info;
//  emit infoChanged();
//}

EventInfo const *SeismEvent::getInfo() const { return _info.get(); }

EventInfo *SeismEvent::getInfo() { return _info.get(); }

void SeismEvent::setName(const QString &name) { _info->name = name; }

const QString &SeismEvent::getName() const { return _info->name; }

const QDateTime &SeismEvent::getAddedDateTime() const {
  return _info->addedDateTime;
}

void SeismEvent::setType(int type) { _info->type = type; }

int SeismEvent::getType() const { return _info->type; }

void SeismEvent::setStampDateTime(const QDateTime &dateTime) {
  _info->stampDateTime = dateTime;
}

const QDateTime &SeismEvent::getStampDateTime() const {
  return _info->stampDateTime;
}

int SeismEvent::getPickAmountByType(const SeismWavePick::Type type) const {
  int amount = 0;
  for (auto &component : _components) {
    amount += component->containsWavePickBy(type) ? 1 : 0;
  }
  return amount;
}

int SeismEvent::getComponentAmount() const {
  return static_cast<int>(_components.size());
}

void SeismEvent::addComponent(
    const std::shared_ptr<SeismComponent> &component) {
  auto &componentStampDateTime = component->getInfo().getStampDateTime();
  if (!_info->stampDateTime.isValid()) {
    //    _info->stampDateTime = componentStampDateTime;
    _info->setStampDateTime(componentStampDateTime);
    //    emit infoChanged(_info.get());
  } else if (_info->stampDateTime > componentStampDateTime) {
    //    _info->stampDateTime = componentStampDateTime;
    _info->setStampDateTime(componentStampDateTime);

    //    emit infoChanged(_info.get());
  }

  // TODO: проверить правильно ли?
  connect(component.get(), &SeismComponent::infoChanged, [this, &component]() {
    auto &componentStampDateTime = component->getInfo().getStampDateTime();
    if (!_info->stampDateTime.isValid()) {
      //      _info->stampDateTime = componentStampDateTime;
      _info->setStampDateTime(componentStampDateTime);
      //      emit infoChanged(_info.get());
    } else if (_info->stampDateTime > componentStampDateTime) {
      //      _info->stampDateTime = componentStampDateTime;
      _info->setStampDateTime(componentStampDateTime);

      //      emit infoChanged(_info.get());
    }
  });

  connect(component.get(), &SeismComponent::dataChanged,
          [this]() { emit dataChanged(); });

  _components.push_back(component);

  emit dataChanged();
}

bool SeismEvent::removeComponentByReceiverUuid(const QUuid &receiverUuid) {
  for (auto itr = _components.begin(); itr != _components.end(); ++itr) {
    if (receiverUuid == (*itr)->getReceiverUuid()) {
      _components.erase(itr);
      emit dataChanged();
      return true;
    }
  }
  return false;
}

const std::list<std::shared_ptr<SeismComponent>> &
SeismEvent::getComponents() const {
  return _components;
}

bool SeismEvent::isTransformBy(TransformOperation oper) const {
  return _appliedOperations.end() != _appliedOperations.find(oper);
}

void SeismEvent::process() {
  _location = {1.67, 1.113, 1.13}; // TODO: implement!
  _isProcessed = true;
  emit dataChanged();
}

bool SeismEvent::isProcessed() const { return _isProcessed; }

const Point &SeismEvent::getLocation() const { return _location; }

QJsonObject &SeismEvent::writeToJson(QJsonObject &json, const QDir &dir) {
  if (_path.isEmpty()) {
    _path = _default_path;
    _path += _uuid.toString();
    _path += ".bin";
  }

  json["name"] = _info->name;
  json["type"] = _info->type;
  json["path"] = _path;
  json["addedDateTime"] = _info->addedDateTime.toString("dd.MM.yy hh:mm:ss");
  json["stampDateTime"] =
      _info->stampDateTime.toString("dd.MM.yy hh:mm:ss:zzz");
  json["isProcessed"] = _isProcessed;
  if (_isProcessed) {
    QJsonArray locationArray;
    locationArray.append(static_cast<double>(std::get<0>(_location)));
    locationArray.append(static_cast<double>(std::get<1>(_location)));
    locationArray.append(static_cast<double>(std::get<2>(_location)));
    json["Location"] = locationArray;
  }

  SeismComponentWriter writer(QFileInfo(dir, _path));

  QJsonArray componentsArray;
  QJsonObject componentObj;
  for (auto &component : _components) {
    componentsArray.append(component->writeToJson(componentObj));
    writer.writeComponent(component);
  }

  json["Components"] = componentsArray;

  return json;
}

void SeismEvent::addTransformOperation(TransformOperation oper) {
  _appliedOperations.insert(oper);
  //  emit dataChanged();
}

void SeismEvent::removeTransformOperation(TransformOperation oper) {
  _appliedOperations.erase(oper);
  //   emit dataChanged();
}

EventInfo::EventInfo(QObject *parent)
    : QObject(parent), addedDateTime(QDateTime::currentDateTime()) {}

EventInfo::EventInfo(const EventInfo &other)
    : name(other.name), type(other.type), addedDateTime(other.addedDateTime),
      stampDateTime(other.stampDateTime) {}

void EventInfo::setName(const QString &name) {
  this->name = name;
  emit changed();
}

const QString &EventInfo::getName() const { return name; }

void EventInfo::setType(int type) {
  this->type = type;
  emit changed();
}

int EventInfo::getType() const { return type; }

void EventInfo::setAddedDateTime(const QDateTime &addedDateTime) {
  this->addedDateTime = addedDateTime;
  emit changed();
}

const QDateTime &EventInfo::getAddedDateTime() const { return addedDateTime; }

void EventInfo::setStampDateTime(const QDateTime &stampDateTime) {
  this->stampDateTime = stampDateTime;
  emit changed();
}

const QDateTime &EventInfo::getStampDateTime() const { return stampDateTime; }

} // namespace Data
