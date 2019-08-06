#include "seismevent.h"

#include "io/seismcomponentreader.h"
#include "io/seismcomponentwriter.h"

#include <QJsonArray>


typedef Data::IO::SeismComponentReader SeismComponentReader;
typedef Data::IO::SeismComponentWriter SeismComponentWriter;


namespace Data {
SeismEvent::SeismEvent()
    :_dateTime(QDateTime::currentDateTime())
{}

SeismEvent::SeismEvent(const QJsonObject& json, const QFileInfo& fileInfo)
{
    if( !(json.contains("date") && json.contains("Components")) ) {
        throw std::runtime_error("Not found json-field (SeismEvent)");
    }

    _dateTime = QDateTime::fromString( json["date"].toString(), "dd.MM.yy hh:mm:ss" );
    QJsonArray componentsArray( json["Components"].toArray() );

    SeismComponentReader reader(fileInfo);
    reader.next();

    for(auto objComponent : componentsArray) {
        auto seismComponent = std::make_unique<SeismComponent>(objComponent.toObject(), reader.getData());
        _components.push_back(std::move(seismComponent));

        reader.next();
    }
}

void SeismEvent::addComponent(std::unique_ptr<SeismComponent>& component)
{
    _components.push_back(std::move(component));
}

int SeismEvent::getComponentNumber() const
{
    return static_cast<int>(_components.size());
}

const std::unique_ptr<SeismComponent>& SeismEvent::getComponent(int idx) const
{
    assert(0 <= idx && idx < getComponentNumber());

    return _components[static_cast<unsigned>(idx)];
}

const std::vector<std::unique_ptr<SeismComponent>>& SeismEvent::getComponents() const
{
    return _components;
}

void SeismEvent::setDateTime(const QDateTime& dateTime)
{
    _dateTime = dateTime;
}

const QDateTime& SeismEvent::getDateTime() const
{
    return _dateTime;
}

QJsonObject& SeismEvent::writeToJson(QJsonObject& json, const QFileInfo& fileInfo) const
{
    json["date"] = _dateTime.toString( "dd.MM.yy hh:mm:ss" );

    SeismComponentWriter writer(fileInfo, getComponentNumber(), 3);

    QJsonArray componentsArray;
    QJsonObject componentObj;
    for(const std::unique_ptr<SeismComponent>& component : _components) {
        componentsArray.append( component->writeToJson(componentObj) );
        writer.writeComponent(component);
    }

    json["Components"] = componentsArray;

    return json;
}


} // namespace Data
