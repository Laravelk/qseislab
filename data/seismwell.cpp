#include "seismwell.h"

#include "data/io/pointreader.h"
#include "data/io/pointwriter.h"

typedef Data::IO::PointReader PointReader;
typedef Data::IO::PointWriter PointWriter;

namespace Data {
const QString SeismWell::_default_path = "data/wells/";

SeismWell::SeismWell() {}

SeismWell::SeismWell(const QJsonObject &json, const QDir &dir) {
  std::string err_msg;

  if (json.contains("name")) {
    _name = json["name"].toString();
  } else {
    err_msg += "::name : not found\n";
  }

  if (json.contains("path")) {
    _path = json["path"].toString();
  } else {
    err_msg += "::path : not found\n";
  }

  QFileInfo fileInfo(dir, _path);
  if (fileInfo.exists()) {
    _uuid = fileInfo.baseName();

    PointReader reader(fileInfo);
    while (reader.hasNext()) {
      _points.push_back(reader.next());
    }

    if (json.contains("pointNumber")) {
      int jPointNum = json["pointNumber"].toInt();
      int realPointNum = static_cast<int>(_points.size());
      if (jPointNum != realPointNum) {
        err_msg += "::pointNumber : json-value != real-size  "
                   "(Note: real-size == " +
                   std::to_string(_points.size()) + ")\n";
      }
    } else {
      err_msg += "::pointNumber : not found  (Note: real value == " +
                 std::to_string(_points.size()) + ")\n";
    }

  } else {
    err_msg += "::data-file : doesn`t exist\n";
  }

  if (!err_msg.empty()) {
    err_msg += "\n";
    throw std::runtime_error(err_msg);
  }
}

void SeismWell::setName(const QString &name) { _name = name; }

const QString &SeismWell::getName() const { return _name; }

int SeismWell::getPointsNumber() const {
  return static_cast<int>(_points.size());
}

void SeismWell::addPoint(const Point &point) { _points.push_back(point); }

const Point &SeismWell::getPoint(int idx) {
  assert(0 <= idx && idx < getPointsNumber());

  return _points[static_cast<unsigned>(idx)];
}

const std::vector<Point> &SeismWell::getPoints() { return _points; }

void SeismWell::setUuid(const QUuid &uuid) { _uuid = uuid; }

const QUuid &SeismWell::getUuid() const { return _uuid; }

QJsonObject &SeismWell::writeToJson(QJsonObject &json, const QDir &dir) {
  if (_path.isEmpty()) {
    _path = _default_path;
    _path += _uuid.toString();
    _path += ".bin";
  }

  json["name"] = _name;
  json["path"] = _path;
  json["pointNumber"] = getPointsNumber();

  PointWriter writer(QFileInfo(dir, _path));
  for (auto point : _points) {
    writer.writePoint(point);
  }

  return json;
}

} // namespace Data
