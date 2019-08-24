#include "controller.h"

#include "data/seismproject.h"
#include "model.h"

typedef Data::SeismProject SeismProject;

namespace ProjectOperation {
namespace OpenProject {
Controller::Controller(QObject *parent)
    : QObject(parent), _model(new Model(this)),
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
  _project = _model->getSeismProjectFrom(path);
  _view->update(_project);
}

void Controller::recvNotification(const QString &msg) {
  _view->setNotification(msg);
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    _view->settingProjectInfo(_project);
    emit sendProject(_project);
  }

  emit finished();
}

} // namespace OpenProject
} // namespace ProjectOperation
