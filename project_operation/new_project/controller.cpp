#include "controller.h"

#include "data/seismproject.h"
#include "view.h"

typedef Data::SeismProject SeismProject;

namespace ProjectOperation {
namespace NewProject {
Controller::Controller(QObject *parent)
    : QObject(parent), _project(std::make_unique<SeismProject>()),
      _view(std::make_unique<View>()) {
  connect(_view.get(), SIGNAL(finished(int)), this, SLOT(finish(int)));

  _view->setModal(true);
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    _project->setName(_view->getName());
    _project->setDate(_view->getDate());
    _project->setTime(_view->getTime());
    emit sendProject(_project);
  }

  emit finished();
}

} // namespace NewProject
} // namespace ProjectOperation
