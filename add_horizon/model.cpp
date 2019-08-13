#include "model.h"

#include "data/io/seismpointreader.h"

typedef Data::SeismHorizon SeismHorizon;
typedef Data::IO::SeismPointReader SeismPointReader;

namespace AddHorizon {
Model::Model(QObject *parent) : QObject(parent) {}

std::unique_ptr<Data::SeismHorizon>
Model::getSeismHorizonFrom(const QString &path) {
  _horizon = std::make_unique<SeismHorizon>();

  try {
    SeismPointReader reader(path);

    while (reader.hasNext()) {
      _horizon->addPoint(reader.getPoint());
      reader.next();
    }
  } catch (const std::runtime_error &err) {
    _horizon.reset();
    emit notify(QString::fromStdString(err.what()));
  }

  return std::move(_horizon);
}

} // namespace AddHorizon
