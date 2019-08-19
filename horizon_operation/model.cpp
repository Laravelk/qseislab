#include "model.h"

#include "data/io/seismpointreader.h"
#include "data/seismhorizon.h"

typedef Data::SeismHorizon SeismHorizon;
typedef Data::IO::SeismPointReader SeismPointReader;

namespace HorizonOperation {
Model::Model(QObject *parent) : QObject(parent) {}

std::unique_ptr<SeismHorizon> Model::getSeismHorizonFrom(const QString &path) {
  std::unique_ptr<SeismHorizon> horizon = std::make_unique<SeismHorizon>();

  QFileInfo fileInfo(path);
  horizon->setName(fileInfo.baseName());

  try {
    SeismPointReader reader(path);

    while (reader.hasNext()) {
      horizon->addPoint(reader.next());
    }
  } catch (const std::runtime_error &err) {
    horizon.reset();
    emit notify(QString::fromStdString(err.what()));
  }

  return horizon;
}

} // namespace HorizonOperation
