#include "model.h"

#include "data/io/abstractsegyreader.h"
#include "data/seismcomponent.h"
#include "data/seismevent.h"
#include "data/seismtrace.h"

typedef Data::IO::AbstractSegyReader AbstractSegyReader;
typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;

namespace EventOperation {
namespace AddEvent {
Model::Model(AbstractSegyReader *reader, QObject *parent)
    : QObject(parent), _reader(reader) {}

std::unique_ptr<Data::SeismEvent>
Model::getSeismEventFrom(const QString &path) {
  _event = std::make_unique<SeismEvent>();

  try {
    _reader->setFilePath(path.toLocal8Bit().data());
    _reader->readBinHeader();

    while (_reader->hasNextComponent()) {
      std::unique_ptr<SeismComponent> component =
          std::make_unique<SeismComponent>();

      _event->addComponent(_reader->nextComponent());
    }
  } catch (const std::runtime_error &err) {
    _event.reset();
    emit notify(QString::fromStdString(err.what()));
  }

  _reader->close();

  return std::move(_event);
}

Model::~Model() { delete _reader; }

} // namespace AddEvent
} // namespace EventOperation
