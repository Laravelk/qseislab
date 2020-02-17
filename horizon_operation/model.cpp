#include "model.h"

#include "data/io/horizonreadergrd.h"
#include "data/io/pointreader.h"
#include "data/seismhorizon.h"

typedef Data::SeismHorizon SeismHorizon;
typedef Data::IO::PointReader PointReader;
typedef Data::IO::HorizonReaderGRD GrdReader;

namespace HorizonOperation {
Model::Model(QObject *parent) : QObject(parent) {}

std::unique_ptr<SeismHorizon> Model::getSeismHorizonFrom(const QString &path) {
  std::unique_ptr<SeismHorizon> horizon = std::make_unique<SeismHorizon>();

  QFileInfo fileInfo(path);
  horizon->setName(fileInfo.baseName());

  try {
    if (".grd" == fileInfo.suffix()) {
      GrdReader reader(path);

      while (reader.hasNext()) {
        horizon->addPoint(reader.next());
      }

      horizon->setNx(reader.getNx());
      horizon->setNy(reader.getNy());
    } else {
      PointReader reader(path);

      while (reader.hasNext()) {
        horizon->addPoint(reader.next());
      }
    }
  } catch (const std::runtime_error &err) {
    horizon.reset();
    emit notify(QString::fromStdString(err.what()));
  }

  return horizon;
}

} // namespace HorizonOperation
