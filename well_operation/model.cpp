#include "model.h"

#include "data/io/pointreader.h"
#include "data/seismwell.h"

typedef Data::SeismWell SeismWell;
typedef Data::IO::PointReader PointReader;

namespace WellOperation {
Model::Model(QObject *parent) : QObject(parent) {}

std::unique_ptr<SeismWell> Model::getSeismWellFrom(const QString &path) {
  std::unique_ptr<SeismWell> well = std::make_unique<SeismWell>();

  QFileInfo fileInfo(path);
  well->setName(fileInfo.baseName());

  try {
    PointReader reader(path);

    while (reader.hasNext()) {
      well->addPoint(reader.next());
    }
  } catch (const std::runtime_error &err) {
    well.reset();
    emit notify(QString::fromStdString(err.what()));
  }

  return well;
}

} // namespace WellOperation
