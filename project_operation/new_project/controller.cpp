#include "controller.h"

#include "data/seismproject.h"
#include "view.h"

typedef Data::SeismProject SeismProject;

namespace ProjectOperation {
namespace NewProject {
Controller::Controller(QObject *parent)
    : QObject(parent), _view(std::make_unique<View>()),
      _project(std::make_unique<SeismProject>()) {

  connect(_view.get(), SIGNAL(finished(int)), this, SLOT(finish(int)));
}

void Controller::start() {
  _view->setModal(true);
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    _view->settingProjectInfo(_project);
    emit sendProject(_project);
  }

  emit finished();
}

} // namespace NewProject
} // namespace ProjectOperation
