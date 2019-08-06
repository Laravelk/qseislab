#include "seismproject.h"

#include <QDir>
#include <QJsonArray>


namespace Data {
SeismProject::SeismProject(QObject *parent)
    :QObject(parent)
{}

SeismProject::SeismProject(const QJsonObject& json, const QFileInfo& fileInfo, QObject* parent)
    :QObject(parent),
     _fileInfo(fileInfo)
{
    if( !(json.contains("Events") && json.contains("Horizons") && json.contains("name") && json.contains("date")) ) {
        throw std::runtime_error("Not found json-field (SeismProject)");
    }

    _name = json["name"].toString();
    _dateTime = QDateTime::fromString( json["date"].toString(), "dd.MM.yy hh:mm:ss" );

    QDir dataDir = _fileInfo.dir();
    if( !dataDir.cd("data") ){
        throw std::runtime_error("Not found data-dir");
    }
    if( !dataDir.cd("events") ){
        throw std::runtime_error("Not found data-events-dir");
    }

    QJsonArray eventsArray( json["Events"].toArray() );
    unsigned num = 0;
    for(auto objEvent : eventsArray) {
        // NOTE: тут "определяется" формат, в котором хранятся данные (.bin)
        QFileInfo dataFileInfo(dataDir, QString::number(num++) + ".bin");
        auto seismEvent = std::make_unique<SeismEvent>(objEvent.toObject(), dataFileInfo);
        _events.push_back(std::move(seismEvent));
    }

    dataDir.cdUp();
    if( !dataDir.cd("horizons") ){
        throw std::runtime_error("Not found data-horizons-dir");
    }

    QJsonArray horizonssArray( json["Horizons"].toArray() );
    num = 0;
    for(auto objHorizon : horizonssArray) {
        // NOTE: тут "определяется" формат, в котором хранятся данные (.bin)
        QFileInfo dataFileInfo(dataDir, QString::number(num++) + ".bin");
        auto seismHorizon = std::make_unique<SeismHorizon>(objHorizon.toObject(), dataFileInfo);
        _horizons.push_back(std::move(seismHorizon));
    }

    _isSaved = true;
}

bool SeismProject::exist() const
{
    // TODO: it`s OK?
    return _fileInfo.isFile();
}

bool SeismProject::isSaved() const
{
    return _isSaved;
}

QJsonObject& SeismProject::writeToJson(QJsonObject& json, const QFileInfo& fileInfo)
{
    assert(fileInfo.isFile());

    _fileInfo = fileInfo;

    json["name"] = _name;
    json["date"] = _dateTime.toString( "dd.MM.yy hh:mm:ss" );

    QDir dataDir = _fileInfo.dir();
    dataDir.mkdir("data");
    dataDir.cd("data");
    dataDir.mkdir("events");
    dataDir.cd("events");

    QJsonArray eventsArray;
    QJsonObject eventObj;
    unsigned num = 0;
    for(const std::unique_ptr<SeismEvent>& event : _events) {
        // NOTE: тут "определяется" формат, в котором хранятся данные (.bin)
        QFileInfo dataFileInfo(dataDir, QString::number(num++) + ".bin");
        eventsArray.append( event->writeToJson(eventObj, dataFileInfo) );
    }

    json["Events"] = eventsArray;


    dataDir.cdUp();
    dataDir.mkdir("horizons");
    dataDir.cd("horizons");

    QJsonArray horizonsArray;
    QJsonObject horizonObj;
    num = 0;
    for(const std::unique_ptr<SeismHorizon>& horizon : _horizons) {
        // NOTE: тут "определяется" формат, в котором хранятся данные (.bin)
        QFileInfo dataFileInfo(dataDir, QString::number(num++) + ".bin");
        horizonsArray.append( horizon->writeToJson(horizonObj, dataFileInfo) );
    }

    json["Horizons"] = horizonsArray;


    _isSaved = true;

    return json;
}

void SeismProject::setName(const QString& name)
{
    if(name != _name) {
        _isSaved = false;
        _name = name;
    }
}

const QString& SeismProject::getName() const
{
    return _name;
}

void SeismProject::setDate(const QDate& date)
{
    if(date != _dateTime.date()) {
        _isSaved = false;
        _dateTime.setDate(date);
    }
}

void SeismProject::setTime(const QTime& time)
{
    if(time != _dateTime.time()){
        _isSaved = false;
        _dateTime.setTime(time);
    }
}

void SeismProject::setDateTime(const QDateTime& dateTime)
{
    if(dateTime != _dateTime){
        _isSaved = false;
        _dateTime = dateTime;
    }
}

const QDateTime& SeismProject::getDateTime() const
{
    return _dateTime;
}

void SeismProject::setFileInfo(const QFileInfo& fileInfo)
{
    if(fileInfo != _fileInfo) {
        _isSaved = false;
        _fileInfo = fileInfo;
    }
}

const QFileInfo& SeismProject::getFileInfo()
{
    return _fileInfo;
}

void SeismProject::addEvent(std::unique_ptr<SeismEvent>& event)
{
    _isSaved = false;
    _events.push_back(std::move(event));
    emit changed();
}

int SeismProject::getEventsNumber() const
{
    return static_cast<int>(_events.size());
}

const std::vector<std::unique_ptr<SeismEvent>>& SeismProject::getEvents() const
{
    return _events;
}

void SeismProject::addHorizon(std::unique_ptr<SeismHorizon>& horizon)
{
    _isSaved = false;
    _horizons.push_back(std::move(horizon));
    emit changed();
}

int SeismProject::getHorizonsNumber() const
{
    return static_cast<int>(_horizons.size());
}

const std::vector<std::unique_ptr<SeismHorizon> > &SeismProject::getHorizons() const
{
    return _horizons;
}


} // namespace Data
