#include "controller.h"

#include "data/seismproject.h"

#include <QMessageBox>

#include <assert.h>

typedef Data::SeismProject SeismProject;

namespace ProjectOperation {
namespace CloseProject {
Controller::Controller(QObject *parent) : QObject(parent) {}

void Controller::closeProject(
    const std::shared_ptr<SeismProject> &project) const {
  assert(project);

  QMessageBox *view = new QMessageBox();
  if (project->isSaved()) {
    view->setText("Project is saved.");
    view->setInformativeText("Close the project?");
    view->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    view->setDefaultButton(QMessageBox::Cancel);

    int ret = view->exec();
    switch (ret) {
    case QMessageBox::Ok:
      emit finished(true);
      break;
    case QMessageBox::Cancel:
      emit finished(false);
      break;
    default:
      break;
    }
    return;
  } else {
    view->setText("Project is not saved.");
    view->setInformativeText("Save the project?");
    view->setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                             QMessageBox::Cancel);
    view->setDefaultButton(QMessageBox::Save);

    int ret = view->exec();
    switch (ret) {
    case QMessageBox::Save:
      emit needSaveProject();
      break;
    case QMessageBox::Discard:
      emit finished(true);
      break;
    case QMessageBox::Cancel:
      emit finished(false);
      break;
    default:
      break;
    }
  }
}

void Controller::finish(bool close) { emit finished(close); }

} // namespace CloseProject
} // namespace ProjectOperation
