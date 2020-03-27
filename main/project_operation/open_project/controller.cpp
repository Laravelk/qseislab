#include "controller.h"

#include "data/seismproject.h"
#include "model.h"

typedef Data::SeismProject SeismProject;

namespace ProjectOperation {
namespace OpenProject {
Controller::Controller(QObject *parent)
    : QObject(parent), _model(new Model(this)),
      _view(std::make_unique<View>()) {

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::sendFilePath, [this](auto &path) {
    _project = _model->getSeismProjectFrom(path);
    _view->update(_project.get());
  });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::start() {
  _view->setModal(true);
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    _view->settingProjectInfo(_project.get());
    emit sendProject(_project);
  }

  emit finished();
}

} // namespace OpenProject
} // namespace ProjectOperation
