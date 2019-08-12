#include "seismhorizon.h"

#include "data/io/seismpointreader.h"
#include "data/io/seismpointwriter.h"


typedef Data::IO::SeismPointReader SeismPointReader;
typedef Data::IO::SeismPointWriter SeismPointWriter;


namespace Data {
const QString SeismHorizon::_default_path = "data/horizons/";

SeismHorizon::SeismHorizon()
{}

SeismHorizon::SeismHorizon(const QJsonObject& json, const QDir& dir)
{
    if( !(json.contains("point number") && json.contains("path")) ) {
        throw std::runtime_error("Not found json-field (SeismHorizon)");
    }

    _path = json["path"].toString();
    QFileInfo fileInfo(dir, _path);
    if(!fileInfo.exists()) {
        throw std::runtime_error("data-file: " + fileInfo.absoluteFilePath().toStdString() + " does not exist");
    }
    // NOTE: хорошая ли идея считывать id из файла или раздавать каждый раз при чтении
    _uuid = fileInfo.baseName();

    int pointNumber = json["point number"].toInt();

    SeismPointReader reader(fileInfo);

    for(int readNumber = 0; readNumber < pointNumber; ++readNumber ) {
        _points.push_back(SeismPoint(reader.getPoint()));

        reader.next();
    }
}

int SeismHorizon::getPointsNumber() const
{
    return static_cast<int>(_points.size());
}

void SeismHorizon::addPoint(const SeismPoint& point)
{
    _points.push_back(point);
}

const SeismHorizon::SeismPoint& SeismHorizon::getPoint(int idx)
{
    assert(0 <= idx && idx < getPointsNumber());

    return _points[static_cast<unsigned>(idx)];
}

const std::vector<SeismHorizon::SeismPoint>& SeismHorizon::getPoints()
{
    return _points;
}

void SeismHorizon::setUuid(const SeismHorizon::Uuid& uuid)
{
    _uuid = uuid;
}

const SeismHorizon::Uuid& SeismHorizon::getUuid() const
{
    return _uuid;
}

QJsonObject& SeismHorizon::writeToJson(QJsonObject& json, const QDir& dir)
{
    if(_path.isEmpty()) {
        _path = _default_path;
        _path += _uuid.toString();
        _path += ".bin";
    }

    json["path"] = _path;

    json["point number"] = static_cast<int>(getPointsNumber());

    SeismPointWriter writer(QFileInfo(dir, _path), getPointsNumber());

    for(auto point : _points) {
        writer.writePoint(point);
    }

    return json;
}


} // namespace Data
