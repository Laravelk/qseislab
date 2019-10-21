#include "seismhorizon.h"

#include "data/io/pointreader.h"
#include "data/io/pointwriter.h"

typedef Data::IO::PointReader PointReader;
typedef Data::IO::PointWriter PointWriter;

namespace Data {
const QString SeismHorizon::_default_path = "data/horizons/";

SeismHorizon::SeismHorizon() : _uuid(QUuid::createUuid()) {}

SeismHorizon::SeismHorizon(const QJsonObject &json, const QDir &dir)
    : _uuid(QUuid::createUuid()) {
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

  if (json.contains("Nx")) {
    _Nx = json["Nx"].toInt();
  } else {
    err_msg += "::Nx : not found\n";
  }

  if (json.contains("Ny")) {
    _Ny = json["Ny"].toInt();
  } else {
    err_msg += "::Ny : not found\n";
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

      if (0 != _Nx && 0 != _Ny && 0 != realPointNum) {
        if (realPointNum != _Nx * _Ny) {
          err_msg += "::Nx,Ny : json-(Nx*Ny) != real-size of horizon\n"
                     "(Note: real-size == " +
                     std::to_string(_points.size()) + ")\n";
        }
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

SeismHorizon::SeismHorizon(const SeismHorizon &other)
    : _uuid(other._uuid), _path(other._path), _name(other._name),
      _Nx(other._Nx), _Ny(other._Ny), _points(other._points) {}

// void SeismHorizon::setUuid(const QUuid &uuid) { _uuid = uuid; }

const QUuid &SeismHorizon::getUuid() const { return _uuid; }

void SeismHorizon::setName(const QString &name) { _name = name; }

const QString &SeismHorizon::getName() const { return _name; }

int SeismHorizon::getPointsNumber() const {
  return static_cast<int>(_points.size());
}

void SeismHorizon::addPoint(Point point) { _points.push_back(point); }

const Point &SeismHorizon::getPoint(int idx) {
  assert(0 <= idx && idx < getPointsNumber());

  return _points[static_cast<unsigned>(idx)];
}

const std::vector<Point> &SeismHorizon::getPoints() { return _points; }

void SeismHorizon::setNx(int Nx) { _Nx = Nx; }

int SeismHorizon::getNx() const { return _Nx; }

void SeismHorizon::setNy(int Ny) { _Ny = Ny; }

int SeismHorizon::getNy() const { return _Ny; }

QJsonObject &SeismHorizon::writeToJson(QJsonObject &json, const QDir &dir) {
  if (_path.isEmpty()) {
    _path = _default_path;
    _path += _uuid.toString();
    _path += ".bin";
  }

  json["name"] = _name;
  json["path"] = _path;
  json["pointNumber"] = getPointsNumber();
  json["Nx"] = _Nx;
  json["Ny"] = _Ny;

  PointWriter writer(QFileInfo(dir, _path));
  for (auto point : _points) {
    writer.writePoint(point);
  }

  return json;
}

} // namespace Data
