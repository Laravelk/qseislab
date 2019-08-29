#include "controller.h"

#include "data/seismevent.h"
#include "model.h"

#include "data/io/segyreader.h"

typedef Data::IO::SegyReader SegyReader;

namespace EventOperation {
namespace AddEvent {
Controller::Controller(
    const std::list<std::unique_ptr<Data::SeismReceiver>> &receivers,
    QObject *parent)
    : QObject(parent), _receivers(receivers),
      _model(new Model(new SegyReader(), this)),
      _view(std::make_unique<View>()) {

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::sendFilePath, [this](auto &path) {
    _event = _model->getSeismEventFrom(path, _receivers);
    _view->update(_event);
  });
  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::start() {
  _view->setModal(true);
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    _view->settingEventInfo(_event);
    emit sendEvent(_event);
  }

  emit finished();
}

} // namespace AddEvent
} // namespace EventOperation
