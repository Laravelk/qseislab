#include "controller.h"

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismProject SeismProject;

using namespace EventOperation;
using namespace ProjectOperation;

namespace Main {
Controller::Controller() : QObject(), _mainWindow(std::make_unique<View>()) {
  connect(_mainWindow.get(), SIGNAL(addEventClicked()), this,
          SLOT(handleAddEventClicked()));
  connect(_mainWindow.get(), SIGNAL(viewEventClicked(const QUuid)), this,
          SLOT(handleViewEventClicked(const QUuid)));
  connect(_mainWindow.get(), SIGNAL(removeEventClicked(const QUuid)), this,
          SLOT(handleRemoveEventClicked(const QUuid)));

  connect(_mainWindow.get(), SIGNAL(horizonsClicked()), this,
          SLOT(handleHorizonsClicked()));

  connect(_mainWindow.get(), SIGNAL(newProjectClicked()), this,
          SLOT(handleNewProjectClicked()));
  connect(_mainWindow.get(), SIGNAL(openProjectClicked()), this,
          SLOT(handleOpenProjectClicked()));
  connect(_mainWindow.get(), SIGNAL(saveProjectClicked()), this,
          SLOT(handleSaveProjectClicked()));
  connect(_mainWindow.get(), SIGNAL(closeProjectClicked()), this,
          SLOT(handleCloseProjectClicked()));

  _mainWindow->show();
}

void Controller::recvProject(std::unique_ptr<Data::SeismProject> &project) {
  assert(project);

  _project = std::move(project);
  connect(_project.get(),
          SIGNAL(addedEvent(const std::unique_ptr<Data::SeismEvent> &)), this,
          SLOT(updateProject(const std::unique_ptr<Data::SeismEvent> &)));
  connect(_project.get(), SIGNAL(removedEvent(const QUuid &)), this,
          SLOT(updateProjectRemoveEvent(const QUuid &)));
  connect(_project.get(),
          SIGNAL(addedHorizon(const std::unique_ptr<Data::SeismHorizon> &)),
          this,
          SLOT(updateProject(const std::unique_ptr<Data::SeismHorizon> &)));
  connect(_project.get(), SIGNAL(removedHorizon(const QUuid &)), this,
          SLOT(updateProjectRemoveHorizon(const QUuid &)));

  _mainWindow->loadProject(_project);
}

void Controller::recvEvent(std::unique_ptr<Data::SeismEvent> &event) {
  assert(_project);
  assert(event);

  _project->addEvent(std::move(event));
}

void Controller::recvHorizon(std::unique_ptr<Data::SeismHorizon> &horizon) {
  assert(_project);
  assert(horizon);

  _project->addHorizon(std::move(horizon));
}

void Controller::updateProject(const std::unique_ptr<Data::SeismEvent> &event) {
  assert(_project);

  _mainWindow->updateProject(event);
}

void Controller::updateProjectRemoveEvent(const QUuid &uuid) {
  assert(_project);

  _mainWindow->updateProjectRemoveEvent(uuid);
}

void Controller::updateProject(
    const std::unique_ptr<Data::SeismHorizon> &horizon) {
  assert(_project);

  _mainWindow->updateProject(horizon);
}

void Controller::updateProjectRemoveHorizon(const QUuid &uuid) {
  assert(_project);

  _mainWindow->updateProjectRemoveHorizon(uuid);
}

void Controller::handleAddEventClicked() {
  if (!_addEventController) {
    _addEventController = std::make_unique<AddEvent::Controller>(this);
    connect(_addEventController.get(),
            SIGNAL(sendEvent(std::unique_ptr<Data::SeismEvent> &)), this,
            SLOT(recvEvent(std::unique_ptr<Data::SeismEvent> &)));
    connect(_addEventController.get(), SIGNAL(finished()), this,
            SLOT(deleteAddEventController()));
  }
}

void Controller::handleViewEventClicked(const QUuid uuid) {
  const std::unique_ptr<SeismEvent> &event = _project->getEvent(uuid);
  if (!_viewEventController) {
    _viewEventController = std::make_unique<ViewEvent::Controller>(this);
    connect(_viewEventController.get(), SIGNAL(finished()), this,
            SLOT(deleteViewEventController()));

    _viewEventController->viewEvent(event);
  }
}

void Controller::handleRemoveEventClicked(const QUuid uuid) {
  _project->removeEvent(uuid);
}

void Controller::handleHorizonsClicked() {
  if (!_horizonController) {
    _horizonController = std::make_unique<HorizonOperation::Controller>(this);
    connect(_horizonController.get(),
            SIGNAL(sendHorizon(std::unique_ptr<Data::SeismHorizon> &)), this,
            SLOT(recvHorizon(std::unique_ptr<Data::SeismHorizon> &)));
    connect(_horizonController.get(), SIGNAL(sendRemovedHorizon(const QUuid &)),
            this, SLOT(handleRemoveHorizonClicked(const QUuid &)));
    connect(_horizonController.get(), SIGNAL(finished()), this,
            SLOT(deleteHorizonController()));

    _horizonController->viewHorizons(_project);
  }
}

void Controller::handleRemoveHorizonClicked(const QUuid &uuid) {
  _project->removeHorizon(uuid);
}

void Controller::handleCloseProjectClicked() {
  if (!_closeProjectController && !_saveProjectController) {
    _closeProjectController = std::make_unique<CloseProject::Controller>(this);
    connect(_closeProjectController.get(), SIGNAL(needSaveProject()), this,
            SLOT(handleSaveProjectClicked()));
    connect(_closeProjectController.get(), SIGNAL(finished(bool)), this,
            SLOT(deleteCloseProjectController(bool)));
    connect(this, SIGNAL(savedProject(bool)), _closeProjectController.get(),
            SLOT(finish(bool)));

    _closeProjectController->closeProject(_project);
  }
}

void Controller::handleNewProjectClicked() {
  if (_project) {
    handleCloseProjectClicked();
    if (_closeProjectController) {
      disconnect(_closeProjectController.get(), SIGNAL(finished(bool)), this,
                 SLOT(deleteCloseProjectController(bool)));
      connect(_closeProjectController.get(), SIGNAL(finished(bool)), this,
              SLOT(adapterFromDeleteToNewProject(bool)));
      return;
    }
  }

  if (!_project) {
    if (!_newProjectController) {
      _newProjectController = std::make_unique<NewProject::Controller>(this);
      connect(_newProjectController.get(),
              SIGNAL(sendProject(std::unique_ptr<Data::SeismProject> &)), this,
              SLOT(recvProject(std::unique_ptr<Data::SeismProject> &)));
      connect(_newProjectController.get(), SIGNAL(finished()), this,
              SLOT(deleteNewProjectController()));
    }
  }
}

void Controller::handleOpenProjectClicked() {
  if (_project) {
    handleCloseProjectClicked();
    if (_closeProjectController) {
      disconnect(_closeProjectController.get(), SIGNAL(finished(bool)), this,
                 SLOT(deleteCloseProjectController(bool)));
      connect(_closeProjectController.get(), SIGNAL(finished(bool)), this,
              SLOT(adapterFromDeleteToOpenProject(bool)));
      return;
    }
  }

  if (!_project) {
    if (!_openProjectController) {
      _openProjectController = std::make_unique<OpenProject::Controller>(this);
      connect(_openProjectController.get(),
              SIGNAL(sendProject(std::unique_ptr<Data::SeismProject> &)), this,
              SLOT(recvProject(std::unique_ptr<Data::SeismProject> &)));
      connect(_openProjectController.get(), SIGNAL(finished()), this,
              SLOT(deleteOpenProjectController()));
    }
  }
}

void Controller::handleSaveProjectClicked() {
  if (!_saveProjectController) {
    _saveProjectController = std::make_unique<SaveProject::Controller>(this);
    connect(_saveProjectController.get(), SIGNAL(finished(bool)), this,
            SLOT(deleteSaveProjectController(bool)));

    _saveProjectController->saveProject(std::move(_project));
  }
}

void Controller::deleteAddEventController() { _addEventController.reset(); }

void Controller::deleteViewEventController() { _viewEventController.reset(); }

void Controller::deleteHorizonController() { _horizonController.reset(); }

void Controller::deleteCloseProjectController(bool closed) {
  _closeProjectController.reset();

  if (closed) {
    _mainWindow->closeProject();
    _project.reset();
  }
}

void Controller::deleteNewProjectController() { _newProjectController.reset(); }

void Controller::deleteOpenProjectController() {
  _openProjectController.reset();
}

void Controller::deleteSaveProjectController(bool saved) {
  _project = _saveProjectController->getProject();
  _saveProjectController.reset();
  emit savedProject(saved);
}

void Controller::adapterFromDeleteToNewProject(bool close) {
  _closeProjectController.reset();

  if (close) {
    _mainWindow->closeProject();
    _project.reset();

    handleNewProjectClicked();
  }
}

void Controller::adapterFromDeleteToOpenProject(bool close) {
  _closeProjectController.reset();

  if (close) {
    _mainWindow->closeProject();
    _project.reset();

    handleOpenProjectClicked();
  }
}

} // namespace Main
