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
  if (!(json.contains("date") && json.contains("Components") &&
        json.contains("path"))) {
    throw std::runtime_error("Not found json-field (SeismEvent)");
  }

  _path = json["path"].toString();
  QFileInfo fileInfo(dir, _path);
  if (!fileInfo.exists()) {
    throw std::runtime_error(
        "data-file: " + fileInfo.absoluteFilePath().toStdString() +
        " does not exist");
  }
  _uuid = fileInfo.baseName();

  _dateTime =
      QDateTime::fromString(json["date"].toString(), "dd.MM.yy hh:mm:ss");

  QJsonArray componentsArray(json["Components"].toArray());

  SeismComponentReader reader(fileInfo);

  for (auto objComponent : componentsArray) {

    if (!reader.hasNext()) {
      throw std::runtime_error("Not enough data for event (SeismEvent)");
    }

    auto seismComponent = std::make_unique<SeismComponent>(
        objComponent.toObject(), reader.nextData());
    _components.push_back(std::move(seismComponent));
  }
}

void SeismEvent::addComponent(std::unique_ptr<SeismComponent> component) {
  _components.push_back(std::move(component));
}

int SeismEvent::getComponentNumber() const {
  return static_cast<int>(_components.size());
}

const std::unique_ptr<SeismComponent> &SeismEvent::getComponent(int idx) const {
  assert(0 <= idx && idx < getComponentNumber());

  return _components[static_cast<unsigned>(idx)];
}

const std::vector<std::unique_ptr<SeismComponent>> &
SeismEvent::getComponents() const {
  return _components;
}

void SeismEvent::setDateTime(const QDateTime &dateTime) {
  _dateTime = dateTime;
}

const QDateTime &SeismEvent::getDateTime() const { return _dateTime; }

void SeismEvent::setUuid(const QUuid &uuid) { _uuid = uuid; }

const QUuid &SeismEvent::getUuid() const { return _uuid; }

QJsonObject &SeismEvent::writeToJson(QJsonObject &json, const QDir &dir) {
  if (_path.isEmpty()) {
    _path = _default_path;
    _path += _uuid.toString();
    _path += ".bin";
  }

  json["path"] = _path;
  json["date"] = _dateTime.toString("dd.MM.yy hh:mm:ss");

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
