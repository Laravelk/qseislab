#include "controller.h"

#include "undo_stack_work/customindividualundocommand.h"
#include "undo_stack_work/event_modification/undocommandgetter.h"
#include "undo_stack_work/shareundocommand.h"

#include <assert.h>

#include <iostream> // TODO: remove

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismReceiver SeismReceiver;
typedef Data::SeismWell SeismWell;
typedef Data::SeismProject SeismProject;

using namespace EventOperation;
using namespace ProjectOperation;

namespace Main {
Controller::Controller(QObject *parent)
    : QObject(parent), _shareEventStack(std::make_unique<QUndoStack>()),
      _mainWindow(std::make_unique<View>()) {

  _mainWindow->updateUndoStack(_shareEventStack.get());

  // share-undo/redo connecting
  connect(_mainWindow.get(), &View::changeEventFocus, [this](auto &eventFocus) {
    if (!_currentOneEventFocus.isNull()) {
      _currentOneEventFocus = QUuid();
    }
    _eventFocus = eventFocus;
    //    if (1 == _eventFocus.size()) {
    //      std::shared_ptr<CustomIndividualUndoStack> &stack =
    //          _eventStacks[*_eventFocus.begin()];
    //      _mainWindow->updateUndoStack(stack.get());
    //            _mainWindow->updateUndoStack(_eventStacks[*_eventFocus.begin()]);
    //    } else {
    _mainWindow->updateUndoStack(_shareEventStack.get());
    //    }
  });
  connect(_mainWindow.get(), &View::changeEventFocusToOne,
          [this](auto &eventUuid) {
            _currentOneEventFocus = eventUuid;
            _mainWindow->updateUndoStack(_eventStacks[eventUuid].get());
          });

  connect(_mainWindow.get(), &View::undoClicked, [this]() {
    if (!_currentOneEventFocus.isNull()) {
      _eventStacks[_currentOneEventFocus]->undo();
    } else {
      _shareEventStack->undo();
    }
  });
  connect(_mainWindow.get(), &View::redoClicked, [this]() {
    if (!_currentOneEventFocus.isNull()) {
      _eventStacks[_currentOneEventFocus]->redo();
    } else {
      _shareEventStack->redo();
    }
  });
  connect(_mainWindow.get(), &View::eventTransformClicked, [this](auto oper) {
    //    std::cout << "here" << std::endl;

    if (!_currentOneEventFocus.isNull()) {
      auto command = UndoCommandGetter::get(
          oper, QUuid(),
          _project->get<SeismEvent>(_currentOneEventFocus).get());
      _eventStacks[_currentOneEventFocus]->push(command);
    } else {

      if (!_eventFocus.empty()) {
        //      auto shareCommand = new ShareUndoCommand(_eventFocus, oper);
        auto shareCommand = new ShareUndoCommand(_eventFocus);
        _shareEventStack->push(shareCommand);

        auto shareUuid = shareCommand->getUuid();

        //      std::cout << "here2" << std::endl;

        // TODO: откуда здесь принимать аргументы и как их вставлять????

        for (auto &eventUuid : _eventFocus) {
          //        std::cout << "create individual command" << std::endl;
          auto command = UndoCommandGetter::get(
              oper, shareUuid, _project->get<SeismEvent>(eventUuid).get());
          _eventStacks[eventUuid]->push(command);
        }

        connect(shareCommand, &ShareUndoCommand::applyUndo,
                [this](auto &shareUuid, auto &eventUuids) {
                  //                  std::cout << "applyUndo" << std::endl;

                  std::set<QUuid> uuidsForRemove;

                  for (auto &eventUuid : eventUuids) {
                    auto &stack = _eventStacks[eventUuid];
                    bool undoApplied = stack->tryUndo(shareUuid);
                    if (!undoApplied) {
                      uuidsForRemove.insert(eventUuid);
                    }
                  }

                  for (auto uuid : uuidsForRemove) {
                    eventUuids.erase(uuid);
                  }
                });
        connect(shareCommand, &ShareUndoCommand::applyRedo,
                [this](auto &shareUuid, auto &eventUuids) {
                  std::set<QUuid> uuidsForRemove;

                  for (auto &eventUuid : eventUuids) {
                    auto &stack = _eventStacks[eventUuid];
                    bool redoApplied = stack->tryRedo(shareUuid);
                    if (!redoApplied) {
                      uuidsForRemove.insert(eventUuid);
                    }
                  }

                  for (auto uuid : uuidsForRemove) {
                    eventUuids.erase(uuid);
                  }
                });
      }
    }
  });

  connect(_mainWindow.get(), &View::addEventsClicked, this,
          &Controller::handleAddEventsClicked);
  connect(_mainWindow.get(), &View::addEventClicked, this,
          &Controller::handleAddEventClicked);

  connect(_mainWindow.get(), &View::eventPageChanged,
          [this](auto &uuid) { _currentOneEventFocus = uuid; });
  connect(_mainWindow.get(), &View::eventPageClosed,
          [this](auto &uuid) { _oneViewEventControllers.erase(uuid); });

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
          [this]() { _mainWindow->viewAboutProject(_project.get()); });

  _mainWindow->show();
}

void Controller::recvProject(const std::shared_ptr<SeismProject> &project) {
  assert(project);

  _project = project;

  // Event`s connecting
  connect(_project.get(), &SeismProject::addedEvent, [this](auto &event) {
    //    _eventStacks[event->getUuid()] =
    //    std::make_unique<CustomUndoStack>();
    _mainWindow->addEvent(event.get());
  });
  connect(_project.get(), &SeismProject::removedEvent, [this](auto &uuid) {
    //    _eventStacks.erase(uuid);
    _mainWindow->removeEvent(uuid);
  });
  connect(_project.get(), &SeismProject::processedEvents, [this] {
    // TODO:
    //  апдейтить каждый эвент, а не загружать каждый
    _mainWindow->processedEvents(_project->getAllMap<SeismEvent>());
  });
  //  connect(_project.get(), &SeismProject::updatedEvent,
  //          [this](auto &event) { _mainWindow->updateEvent(event); });

  // Horizon`s connecting
  connect(_project.get(), &SeismProject::addedHorizon,
          [this](auto &horizon) { _mainWindow->addHorizon(horizon.get()); });
  connect(_project.get(), &SeismProject::removedHorizon,
          [this](auto &uuid) { _mainWindow->removeHorizon(uuid); });

  // Well`s connecting
  connect(_project.get(), &SeismProject::addedWell,
          [this](auto &well) { _mainWindow->addWell(well.get()); });
  connect(_project.get(), &SeismProject::removedWell,
          [this](auto &uuid) { _mainWindow->removeWell(uuid); });

  // Receiver`s connecting
  connect(_project.get(), &SeismProject::addedReceiver,
          [this](auto &receiver) { _mainWindow->addReceiver(receiver.get()); });
  connect(_project.get(), &SeismProject::removedReceiver,
          [this](auto &uuid) { _mainWindow->removeReceiver(uuid); });

  // TODO:
  //  удалить метод loadProject - добавлять каждый элемент
  _mainWindow->loadProject(_project.get());
}

void Controller::handleAddEventsClicked() {
  if (!_moreEventsController) {
    //    _moreEventsController = std::make_unique<MoreEvents::Controller>(
    //        _project->getAllMap<SeismEvent>(),
    //        _project->getAllMap<SeismWell>(), this);
    _moreEventsController = std::make_unique<MoreEvents::Controller>(
        _project->getAllMap<SeismEvent>(), _project->getAllMap<SeismWell>(),
        this);

    connect(_moreEventsController.get(),
            &MoreEvents::Controller::sendEventsAndStacks,
            [this](auto &events_map, auto &stacks_map) {
              for (auto &uuid_event : events_map) {
                _project->add(uuid_event.second);
              }
              for (auto &uuid_stack : stacks_map) {
                _eventStacks[uuid_stack.first] = uuid_stack.second;
              }
            });
    connect(_moreEventsController.get(), &MoreEvents::Controller::finished,
            [this] { _moreEventsController.reset(); });

    _moreEventsController->start();
  }
}

void Controller::handleAddEventClicked() {
  if (!_oneAddEventController) {
    //    _oneEventController = std::make_unique<OneEvent::Controller>(
    //        _project->getAllMap<SeismEvent>(),
    //        _project->getAllMap<SeismWell>(), this);
    _oneAddEventController = std::make_unique<OneEvent::Controller>(
        _project->getAllMap<SeismEvent>(), _project->getAllMap<SeismWell>(),
        this);

    //    connect(
    //        _oneEventController.get(), &OneEvent::Controller::sendEvent,
    //        [this](auto &event) {
    //        _project->add<SeismEvent>(std::move(event));
    //        });
    connect(_oneAddEventController.get(),
            &OneEvent::Controller::sendEventAndStack,
            [this](auto &event, auto &undoStack) {
              //              _eventStacks[event->getUuid()] =
              //              std::move(undoStack);
              //              _project->add<SeismEvent>(std::move(event));
              _eventStacks[event->getUuid()] = undoStack;
              _project->add(event);
            });
    connect(_oneAddEventController.get(), &OneEvent::Controller::finished,
            [this] { _oneAddEventController.reset(); });

    _oneAddEventController->start();
  }
}

void Controller::handleViewEventClicked(const QUuid &uuid) {
  if (_oneViewEventControllers.find(uuid) == _oneViewEventControllers.end()) {
    auto &event = _project->get<SeismEvent>(uuid);

    _oneViewEventControllers[uuid] = std::make_unique<ViewEvent::Controller>(
        _project->getAllMap<SeismEvent>(), _project->getAllMap<SeismWell>(),
        event, this);

    auto &viewEventController = _oneViewEventControllers[uuid];
    _mainWindow->addEventPage(viewEventController->getView(), event.get());
  } else {
    _mainWindow->setFocusEventPage(_oneViewEventControllers[uuid]->getView());
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
    _receiverController = std::make_unique<ReceiverOperation::Controller>(this);

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

    _receiverController->viewReceivers(_project->getAllMap<SeismWell>());
  }
}

void Controller::handleWellsClicked() {
  if (!_wellController) {
    _wellController =
        std::make_unique<WellOperation::Controller>(_mainWindow.get(), this);
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
              //              _project = _saveProjectController->getProject();
              _saveProjectController.reset();
              emit savedProject(saved);
            });

    _saveProjectController->saveProject(_project);
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
