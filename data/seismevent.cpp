#include "seismevent.h"

#include "io/seismcomponentreader.h"
#include "io/seismcomponentwriter.h"

#include <QJsonArray>

typedef Data::IO::SeismComponentReader SeismComponentReader;
typedef Data::IO::SeismComponentWriter SeismComponentWriter;

namespace Data {
const QString SeismEvent::_default_path = "data/events/";

SeismEvent::SeismEvent() : _dateTime(QDateTime::currentDateTime()) {}

SeismEvent::SeismEvent(const QJsonObject &json, const QDir &dir) {
  std::string err_msg;

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
        for (auto objComponent : componentsArray) {

          if (!reader.hasNext()) {
            err_msg += "::data : not enough data for event\n";
            break;
          }

          try {
            auto seismComponent = std::make_unique<SeismComponent>(
                objComponent.toObject(), reader.nextData());
            _components.push_back(std::move(seismComponent));
          } catch (std::runtime_error &err) {
            err_msg += "Component (idx: " + std::to_string(idx) + ")\n";
            err_msg += err.what();
          }

          ++idx;
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

int SeismEvent::getComponentNumber() const {
  return static_cast<int>(_components.size());
}

void SeismEvent::addComponent(std::unique_ptr<SeismComponent> component) {
  _components.push_back(std::move(component));
}

const std::list<std::unique_ptr<SeismComponent>> &
SeismEvent::getComponents() const {
  return _components;
}

void SeismEvent::setDateTime(const QDateTime &dateTime) {
  _dateTime = dateTime;
}

const QDateTime &SeismEvent::getDateTime() const { return _dateTime; }

void SeismEvent::setUuid(const QUuid &uuid) { _uuid = uuid; }

const QUuid &SeismEvent::getUuid() const { return _uuid; }

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
  for (const std::unique_ptr<SeismComponent> &component : _components) {
    componentsArray.append(component->writeToJson(componentObj));
    writer.writeComponent(component);
  }

  json["Components"] = componentsArray;

  return json;
}

} // namespace Data
