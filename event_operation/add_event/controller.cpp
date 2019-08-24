#include "controller.h"

#include "data/seismevent.h"
#include "model.h"

#include "data/io/segyreader.h"

typedef Data::IO::SegyReader SegyReader;

namespace EventOperation {
namespace AddEvent {
Controller::Controller(
    const std::list<std::unique_ptr<Data::SeismReciever>> &recievers,
    QObject *parent)
    : QObject(parent), _recievers(recievers),
      _model(new Model(new SegyReader(), this)),
      _view(std::make_unique<View>()) {
  connect(_view.get(), SIGNAL(sendFilePath(const QString &)), this,
          SLOT(recvFilePath(const QString &)));
  connect(_model, SIGNAL(notify(const QString &)), this,
          SLOT(recvNotification(const QString &)));
  connect(_view.get(), SIGNAL(finished(int)), this, SLOT(finish(int)));
}

void Controller::start() {
  _view->setModal(true);
  _view->show();
}

void Controller::recvFilePath(const QString &path) {
  //  _event = _model->getSeismEventFrom(path);
  _event = _model->getSeismEventFrom(path, _recievers);
  _view->update(_event);
}

void Controller::recvNotification(const QString &msg) {
  _view->setNotification(msg);
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
