#include "seismhorizon.h"

#include "data/io/seismpointreader.h"
#include "data/io/seismpointwriter.h"


typedef Data::IO::SeismPointReader SeismPointReader;
typedef Data::IO::SeismPointWriter SeismPointWriter;


namespace Data {
SeismHorizon::SeismHorizon()
{}

SeismHorizon::SeismHorizon(const QJsonObject& json, const QFileInfo& fileInfo)
{
    if( !(json.contains("point number")) ) {
        throw std::runtime_error("Not found json-field (SeismHorizon)");
    }

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

const SeismPoint& SeismHorizon::getPoint(int idx)
{
    assert(0 <= idx && idx < getPointsNumber());

    return _points[static_cast<unsigned>(idx)];
}

const std::vector<SeismPoint> &SeismHorizon::getPoints()
{
    return _points;
}

QJsonObject& SeismHorizon::writeToJson(QJsonObject& json, const QFileInfo& fileInfo) const
{
    json["point number"] = static_cast<int>(getPointsNumber());

    SeismPointWriter writer(fileInfo, getPointsNumber());

    for(auto point : _points) {
        writer.writePoint(point);
    }

    return json;
}


} // namespace Data
