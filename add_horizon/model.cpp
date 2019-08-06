#include "model.h"

#include "data/seismhorizon.h"
#include "data/io/seismpointreader.h"


typedef Data::SeismHorizon SeismHorizon;
typedef Data::IO::SeismPointReader SeismPointReader;


namespace AddHorizon {
Model::Model(QObject* parent)
    :QObject(parent)
{}

void Model::setFilePath(const QString& path)
{
    _path = path;
    getSeismHorizonFrom(_path);
}

std::unique_ptr<Data::SeismHorizon>& Model::getSeismHorizon()
{
    return _horizon;
}

void Model::getSeismHorizonFrom(const QString& path)
{
    try {
        SeismPointReader reader(path);

        _horizon = std::make_unique<SeismHorizon>();

        while(reader.hasNext()) {
            _horizon->addPoint(reader.getPoint());
            reader.next();
        }
    } catch (std::runtime_error err) {
        emit notify(QString::fromStdString(err.what()));
        _horizon.reset();
    }
}


} // namespace AddHorizon
