#include "model.h"

#include "data/io/abstractsegyreader.h"
#include "data/io/segy_files_params.h"
#include "data/seismcomponent.h"
#include "data/seismreceiver.h"
#include "data/seismtrace.h"
#include "data/seismwell.h"

typedef Data::IO::AbstractSegyReader AbstractSegyReader;
typedef Data::SeismComponent SeismComponent;
typedef Data::SeismWell SeismWell;
typedef Data::SeismTrace SeismTrace;

namespace EventOperation {
namespace AddEvent {
Model::Model(AbstractSegyReader *reader, QObject *parent)
    : QObject(parent), _reader(reader) {}

std::list<std::unique_ptr<SeismComponent>>
Model::getSeismComponents(const std::unique_ptr<SeismWell> &well,
                          const QString &path) {
  std::list<std::unique_ptr<SeismComponent>> components;

  try {
    _reader->setFilePath(path.toLocal8Bit().data());
    _reader->readBinHeader();

    auto itr = well->getReceivers().begin();
    while (_reader->hasNextComponent()) {
      if (well->getReceivers().end() == itr) {
        throw std::runtime_error(
            "There are more traces in the segy-file than in receivers");
      }

      components.push_back(_reader->nextComponent(*itr));
      ++itr;
    }
    if (well->getReceivers().end() != itr) {
      throw std::runtime_error(
          "There are more traces in receivers than in the segy-file");
    }
  } catch (const std::runtime_error &err) {
    components.clear();
    emit notify(QString::fromStdString(err.what()));
  }

  _reader->close();

  return components;
}

Model::~Model() { delete _reader; }

} // namespace AddEvent
} // namespace EventOperation
