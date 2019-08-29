#include "model.h"

#include "data/io/abstractsegyreader.h"
#include "data/io/segy_files_params.h"
#include "data/seismcomponent.h"
#include "data/seismevent.h"
#include "data/seismreceiver.h"
#include "data/seismtrace.h"

typedef Data::IO::AbstractSegyReader AbstractSegyReader;
typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;

namespace EventOperation {
namespace AddEvent {
Model::Model(AbstractSegyReader *reader, QObject *parent)
    : QObject(parent), _reader(reader) {}

std::unique_ptr<Data::SeismEvent> Model::getSeismEventFrom(
    const QString &path,
    const std::list<std::unique_ptr<Data::SeismReceiver>> &receivers) {

  std::unique_ptr<Data::SeismEvent> event = std::make_unique<SeismEvent>();

  try {
    _reader->setFilePath(path.toLocal8Bit().data());
    _reader->readBinHeader();

    auto itr = receivers.begin();
    while (_reader->hasNextComponent()) {
      if (receivers.end() == itr) {
        throw std::runtime_error(
            "There are more traces in the segy-file than in receivers");
      }

      event->addComponent(_reader->nextComponent(*itr));
      ++itr;
    }
  } catch (const std::runtime_error &err) {
    event.reset();
    emit notify(QString::fromStdString(err.what()));
  }

  _reader->close();

  return event;
}

Model::~Model() { delete _reader; }

} // namespace AddEvent
} // namespace EventOperation
