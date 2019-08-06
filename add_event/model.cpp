#include "model.h"

#include "data/io/abstractsegyreader.h"
#include "data/io/segyreader.h"
#include "data/seismevent.h"

#include <QDateTime>

#include <memory>


typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::IO::AbstractSegyReader AbstractSegyReader;
typedef Data::IO::SegyReader SegyReader;


namespace AddEvent {
Model::Model(QObject* parent)
    :QObject(parent)
{}

void Model::setFilePath(const QString& path)
{
    _path = path;
    getSeismEventFrom(_path);
    emit changed();
}

std::unique_ptr<Data::SeismEvent>& Model::getSeismEvent()
{
    assert(_event);
    return _event;
}

void Model::getSeismEventFrom(const QString& path)
{
    AbstractSegyReader* sreader = new SegyReader(path.toLocal8Bit().data());
    if(!sreader->isValid()) {
        emit notify(sreader->getErrMsg());
        delete sreader;
        return;
    }

    _event = std::make_unique<SeismEvent>();

    while(sreader->hasNextComponent()) {
        SeismComponent* component = sreader->nextComponent();
        if(!component) {
            emit notify(sreader->getErrMsg());
            break;
        }

        std::unique_ptr<SeismComponent> unq_component(component);
        _event->addComponent(unq_component);
    }
    delete sreader;
}


} // namespace AddEvent
