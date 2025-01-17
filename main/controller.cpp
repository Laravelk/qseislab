#include "controller.h"

//#include <iostream> // TODO: remove

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismReceiver SeismReceiver;
typedef Data::SeismWell SeismWell;
typedef Data::SeismProject SeismProject;

using namespace EventOperation;
using namespace ProjectOperation;

namespace Main {
Controller::Controller(QObject *parent)
    : QObject(parent), _mainWindow(std::make_unique<View>()) {

  connect(_mainWindow.get(), &View::addEventClicked, this,
          &Controller::handleAddEventClicked);
  connect(_mainWindow.get(), &View::viewEventClicked, this,
          &Controller::handleViewEventClicked);
  connect(_mainWindow.get(), &View::removeEventClicked,
          [this](auto uuid) { _project->remove<SeismEvent>(uuid); });
  connect(_mainWindow.get(), &View::processEventsClicked,
          [this] { _project->processEvents(); });

  connect(_mainWindow.get(), &View::horizonsClicked, this,
          &Controller::handleHorizonsClicked);

  connect(_mainWindow.get(), &View::receiversClicked, this,
          &Controller::handleReceiversClicked);

  connect(_mainWindow.get(), &View::wellsClicked, this,
          &Controller::handleWellsClicked);

  connect(_mainWindow.get(), &View::newProjectClicked, this,
          &Controller::handleNewProjectClicked);
  connect(_mainWindow.get(), &View::openProjectClicked, this,
          &Controller::handleOpenProjectClicked);
  connect(_mainWindow.get(), &View::saveProjectClicked, this,
          &Controller::handleSaveProjectClicked);
  connect(_mainWindow.get(), &View::closeProjectClicked, this,
          &Controller::handleCloseProjectClicked);
  connect(_mainWindow.get(), &View::aboutProjectClicked,
          [this]() { _mainWindow->viewAboutProject(_project); });

  _mainWindow->show();
}

void Controller::recvProject(std::unique_ptr<SeismProject> &project) {
  assert(project);

  _project = std::move(project);

  // Event`s connecting
  connect(_project.get(), &SeismProject::addedEvent,
          [this](auto &event) { _mainWindow->addEvent(event); });
  connect(_project.get(), &SeismProject::removedEvent,
          [this](auto &uuid) { _mainWindow->removeEvent(uuid); });
  connect(_project.get(), &SeismProject::processedEvents, [this] {
    _mainWindow->processedEvents(_project->getAllMap<SeismEvent>());
  });
  connect(_project.get(), &SeismProject::updatedEvent,
          [this](auto &event) { _mainWindow->updateEvent(event); });

  // Horizon`s connecting
  connect(_project.get(), &SeismProject::addedHorizon,
          [this](auto &horizon) { _mainWindow->addHorizon(horizon); });
  connect(_project.get(), &SeismProject::removedHorizon,
          [this](auto &uuid) { _mainWindow->removeHorizon(uuid); });

  // Well`s connecting
  connect(_project.get(), &SeismProject::addedWell,
          [this](auto &well) { _mainWindow->addWell(well); });
  connect(_project.get(), &SeismProject::removedWell,
          [this](auto &uuid) { _mainWindow->removeWell(uuid); });

  // Receiver`s connecting
  connect(_project.get(), &SeismProject::addedReceiver,
          [this](auto &receiver) { _mainWindow->addReceiver(receiver); });
  connect(_project.get(), &SeismProject::removedReceiver,
          [this](auto &uuid) { _mainWindow->removeReceiver(uuid); });

  _mainWindow->loadProject(_project);
}

void Controller::handleAddEventClicked() {
  if (!_eventController) {
    _eventController = std::make_unique<Generic::Controller>(
        _project->getAllMap<SeismWell>(), this);

    connect(
        _eventController.get(), &Generic::Controller::sendEvent,
        [this](auto &event) { _project->add<SeismEvent>(std::move(event)); });
    connect(_eventController.get(), &Generic::Controller::finished,
            [this] { _eventController.reset(); });

    _eventController->start();
  }
}

void Controller::handleViewEventClicked(const QUuid uuid) {
  if (!_eventController) {
    _eventController = std::make_unique<Generic::Controller>(
        _project->get<SeismEvent>(uuid), this);

    connect(_eventController.get(), &Generic::Controller::sendEvent,
            [this](auto &event) {
              _project->update<SeismEvent>(std::move(event));
            });

    connect(_eventController.get(), &Generic::Controller::finished,
            [this] { _eventController.reset(); });

    _eventController->start();
  }
}

void Controller::handleHorizonsClicked() {
  if (!_horizonController) {
    _horizonController = std::make_unique<HorizonOperation::Controller>(this);
    connect(_horizonController.get(),
            &HorizonOperation::Controller::sendHorizons,
            [this](auto &horizons_map) {
              _project->setAllMap<SeismHorizon>(horizons_map);
            });

    connect(_horizonController.get(), &HorizonOperation::Controller::finished,
            [this] { _horizonController.reset(); });

    _horizonController->viewHorizons(_project->getAllMap<SeismHorizon>());
  }
}

void Controller::handleReceiversClicked() {
  if (!_receiverController) {
    _receiverController = std::make_unique<ReceiverOperation::Controller>(
        _project->getAllMap<SeismWell>(), this);

    connect(_receiverController.get(),
            &ReceiverOperation::Controller::removeAllReceivers,
            [this]() { _project->removeAllReceivers(); });

    connect(_receiverController.get(),
            &ReceiverOperation::Controller::sendReciver,
            [this](auto &wellUuid, auto &receiver) {
              _project->addReceiver(wellUuid, receiver);
            });

    connect(_receiverController.get(), &ReceiverOperation::Controller::finished,
            [this] { _receiverController.reset(); });

    _receiverController->viewReceivers();
  }
}

void Controller::handleWellsClicked() {
  if (!_wellController) {
    _wellController = std::make_unique<WellOperation::Controller>(this);
    connect(
        _wellController.get(), &WellOperation::Controller::sendWells,
        [this](auto &wells_map) { _project->setAllMap<SeismWell>(wells_map); });
    connect(_wellController.get(), &WellOperation::Controller::finished,
            [this] { _wellController.reset(); });

    _wellController->viewWells(_project->getAllMap<SeismWell>());
  }
}

void Controller::handleCloseProjectClicked() {
  if (!_closeProjectController && !_saveProjectController) {
    _closeProjectController = std::make_unique<CloseProject::Controller>(this);

    connect(_closeProjectController.get(),
            &CloseProject::Controller::needSaveProject, this,
            &Controller::handleSaveProjectClicked);
    connect(_closeProjectController.get(), &CloseProject::Controller::finished,
            this, &Controller::deleteCloseProjectController);
    connect(this, &Controller::savedProject, _closeProjectController.get(),
            &CloseProject::Controller::finish);

    _closeProjectController->closeProject(_project);
  }
}

void Controller::handleNewProjectClicked() {
  if (_project) {
    handleCloseProjectClicked();
    if (_closeProjectController) {
      disconnect(_closeProjectController.get(),
                 &CloseProject::Controller::finished, this,
                 &Controller::deleteCloseProjectController);
      connect(_closeProjectController.get(),
              &CloseProject::Controller::finished, [this](auto close) {
                _closeProjectController.reset();
                if (close) {
                  _mainWindow->closeProject();
                  _project.reset();
                  handleNewProjectClicked();
                }
              });
      return;
    }
  }

  if (!_project) {
    if (!_newProjectController) {
      _newProjectController = std::make_unique<NewProject::Controller>(this);

      connect(_newProjectController.get(), &NewProject::Controller::sendProject,
              this, &Controller::recvProject);
      connect(_newProjectController.get(), &NewProject::Controller::finished,
              [this] { _newProjectController.reset(); });

      _newProjectController->start();
    }
  }
}

void Controller::handleOpenProjectClicked() {
  if (_project) {
    handleCloseProjectClicked();
    if (_closeProjectController) {
      disconnect(_closeProjectController.get(),
                 &CloseProject::Controller::finished, this,
                 &Controller::deleteCloseProjectController);
      connect(_closeProjectController.get(),
              &CloseProject::Controller::finished, [this](auto close) {
                _closeProjectController.reset();
                if (close) {
                  _mainWindow->closeProject();
                  _project.reset();
                  handleOpenProjectClicked();
                }
              });
      return;
    }
  }

  if (!_project) {
    if (!_openProjectController) {
      _openProjectController = std::make_unique<OpenProject::Controller>(this);

      connect(_openProjectController.get(),
              &OpenProject::Controller::sendProject, this,
              &Controller::recvProject);
      connect(_openProjectController.get(), &OpenProject::Controller::finished,
              [this] { _openProjectController.reset(); });

      _openProjectController->start();
    }
  }
}

void Controller::handleSaveProjectClicked() {
  if (!_saveProjectController) {
    _saveProjectController = std::make_unique<SaveProject::Controller>(this);

    connect(_saveProjectController.get(), &SaveProject::Controller::finished,
            [this](bool saved) {
              _project = _saveProjectController->getProject();
              _saveProjectController.reset();
              emit savedProject(saved);
            });

    _saveProjectController->saveProject(std::move(_project));
  }
}

void Controller::deleteCloseProjectController(bool closed) {
  _closeProjectController.reset();

  if (closed) {
    _mainWindow->closeProject();
    _project.reset();
  }
}

} // namespace Main
