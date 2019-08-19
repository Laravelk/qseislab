#include "seismhorizon.h"

#include "data/io/seismpointreader.h"
#include "data/io/seismpointwriter.h"

typedef Data::IO::SeismPointReader SeismPointReader;
typedef Data::IO::SeismPointWriter SeismPointWriter;

namespace Data {
const QString SeismHorizon::_default_path = "data/horizons/";

SeismHorizon::SeismHorizon() {}

SeismHorizon::SeismHorizon(const QJsonObject &json, const QDir &dir) {
  if (!(json.contains("name") && json.contains("path"))) {
    throw std::runtime_error("Not found json-field (SeismHorizon)");
  }

  _name = json["name"].toString();

  _path = json["path"].toString();
  QFileInfo fileInfo(dir, _path);
  if (!fileInfo.exists()) {
    throw std::runtime_error(
        "data-file: " + fileInfo.absoluteFilePath().toStdString() +
        " does not exist");
  }
  _uuid = fileInfo.baseName();

  SeismPointReader reader(fileInfo);
  while (reader.hasNext()) {
    _points.push_back(reader.next());
  }

  if (json.contains("pointNumber")) {
    int jpointNumb = json["point number"].toInt();
    if (jpointNumb != static_cast<int>(_points.size())) {
      // TODO: notify
    }
  } else {
    // TODO: notify
  }
}

void SeismHorizon::setName(const QString &name) { _name = name; }

const QString &SeismHorizon::getName() const { return _name; }

int SeismHorizon::getPointsNumber() const {
  return static_cast<int>(_points.size());
}

void SeismHorizon::addPoint(const SeismPoint &point) {
  _points.push_back(point);
}

const SeismHorizon::SeismPoint &SeismHorizon::getPoint(int idx) {
  assert(0 <= idx && idx < getPointsNumber());

  return _points[static_cast<unsigned>(idx)];
}

const std::vector<SeismHorizon::SeismPoint> &SeismHorizon::getPoints() {
  return _points;
}

void SeismHorizon::setUuid(const QUuid &uuid) { _uuid = uuid; }

const QUuid &SeismHorizon::getUuid() const { return _uuid; }

QJsonObject &SeismHorizon::writeToJson(QJsonObject &json, const QDir &dir) {
  if (_path.isEmpty()) {
    _path = _default_path;
    _path += _uuid.toString();
    _path += ".bin";
  }

  json["name"] = _name;
  json["path"] = _path;
  json["pointNumber"] = getPointsNumber();

  SeismPointWriter writer(QFileInfo(dir, _path));
  for (auto point : _points) {
    writer.writePoint(point);
  }

  return json;
}

} // namespace Data
