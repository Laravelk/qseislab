#include "controller.h"

#include "data/seismproject.h"
#include "view.h"

typedef Data::SeismProject SeismProject;

namespace ProjectOperation {
namespace NewProject {
Controller::Controller(QObject *parent)
    : QObject(parent), _view(std::make_unique<View>()),
      _project(std::make_shared<SeismProject>()) {

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

} // namespace NewProject
} // namespace ProjectOperation
