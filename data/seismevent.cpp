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
    : _uuid(QUuid::createUuid()), _dateTime(QDateTime::currentDateTime()) {}

SeismEvent::SeismEvent(const QJsonObject &json,
                       std::map<QUuid, std::unique_ptr<SeismWell>> &wells_map,
                       const QDir &dir)
    : _uuid(QUuid::createUuid()) {

  std::string err_msg;

  if (json.contains("type")) {
    _type = json["type"].toInt();
  } else {
    err_msg += "::type : not found\n";
  }

  if (json.contains("date")) {
    _dateTime =
        QDateTime::fromString(json["date"].toString(), "dd.MM.yy hh:mm:ss");
  } else {
    err_msg += "::date : not found\n";
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
                std::make_unique<SeismComponent>(objComponent.toObject());
            auto &receiverUuid = seismComponent->getReceiverUuid();
            bool findReceiver = false;
            for (auto &uuid_well : wells_map) {
              for (auto &receiver : uuid_well.second->getReceivers()) {
                if (receiver->getUuid() == receiverUuid) {
                  findReceiver = true;
                  for (int i = 0; i < receiver->getChannelNum(); ++i) {
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
    : _uuid(other._uuid), _type(other._type), _path(other._path),
      _dateTime(other._dateTime), _isProcessed(other._isProcessed),
      _location(other._location) {

  for (auto &component : other._components) {
    _components.push_back(std::make_unique<SeismComponent>(*component));
  }
}

const QUuid &SeismEvent::getUuid() const { return _uuid; }

void SeismEvent::setType(int type) { _type = type; }

int SeismEvent::getType() const { return _type; }

int SeismEvent::getComponentNumber() const {
  return static_cast<int>(_components.size());
}

void SeismEvent::addComponent(std::unique_ptr<SeismComponent> component) {
  _components.push_back(std::move(component));
  connect(_components.back().get(), &SeismComponent::changed,
          [this]() { emit changed(); });
}

bool SeismEvent::removeComponentByReceiverUuid(const QUuid &receiverUuid) {
  for (auto itr = _components.begin(); itr != _components.end(); ++itr) {
    if (receiverUuid == (*itr)->getReceiverUuid()) {
      _components.erase(itr);
      return true;
    }
  }
  return false;
}

const std::list<std::unique_ptr<SeismComponent>> &
SeismEvent::getComponents() const {
  return _components;
}

void SeismEvent::setDateTime(const QDateTime &dateTime) {
  _dateTime = dateTime;
}

const QDateTime &SeismEvent::getDateTime() const { return _dateTime; }

void SeismEvent::process() {
  _location = {1.67, 1.113, 1.13};
  _isProcessed = true;
}

bool SeismEvent::isProcessed() const { return _isProcessed; }

const Point &SeismEvent::getLocation() const { return _location; }

QJsonObject &SeismEvent::writeToJson(QJsonObject &json, const QDir &dir) {
  if (_path.isEmpty()) {
    _path = _default_path;
    _path += _uuid.toString();
    _path += ".bin";
  }

  json["type"] = _type;
  json["path"] = _path;
  json["date"] = _dateTime.toString("dd.MM.yy hh:mm:ss");
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

} // namespace Data
