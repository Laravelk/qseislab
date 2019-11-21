#include "view.h"

#include "infoproject.h"
#include "startpage.h"
#include "workpage.h"

#include "data/seismevent.h"
#include "data/seismproject.h"

#include <assert.h>

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismProject SeismProject;

namespace Main {
View::View(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("MainWindow");

  QAction *act;

  QMenu *fileMenu = new QMenu("&File");
  act = fileMenu->addAction(
      "New Project", [this] { emit newProjectClicked(); }, QKeySequence::New);

  act = fileMenu->addAction(
      "Open Project", [this] { emit openProjectClicked(); },
      QKeySequence::Open);

  act = fileMenu->addAction(
      "Save Project", [this] { emit saveProjectClicked(); },
      QKeySequence::Save);
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);

  act = fileMenu->addAction(
      "Close Project", [this] { emit closeProjectClicked(); },
      QKeySequence::Close);
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);

  act = fileMenu->addAction("About Project",
                            [this] { emit aboutProjectClicked(); });
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);

  QMenu *editMenu = new QMenu("&Edit");
  editMenu->setDisabled(true);
  connect(this, &View::projectPresence, editMenu, &QMenu::setEnabled);
  _undoAction = editMenu->addAction("Undo", [this] { emit undoClicked(); });
  _undoAction->setDisabled(true);
  _redoAction = editMenu->addAction("Redo", [this] { emit redoClicked(); });
  _redoAction->setDisabled(true);

  QMenu *actionMenu = new QMenu("Action");
  actionMenu->addAction("Process Events",
                        [this] { emit processEventsClicked(); });
  //  actionMenu->addAction("Rotate", [this] {
  //    emit eventTransformClicked(
  //        SeismEvent::TransformOperation::RotateDataToEBasis);
  //  });
  actionMenu->addAction("Test Mult", [this] {
    emit eventTransformClicked(SeismEvent::TransformOperation::TestMultiplier);
  });
  editMenu->addMenu(actionMenu);

  QMenu *viewMenu = new QMenu("&Data");
  viewMenu->setDisabled(true);
  connect(this, &View::projectPresence, viewMenu, &QMenu::setEnabled);

  viewMenu->addAction("Horizons", [this] { emit horizonsClicked(); });
  viewMenu->addAction("Receivers", [this] { emit receiversClicked(); });
  viewMenu->addAction("Wells", [this] { emit wellsClicked(); });

  QMenu *eventMenu = new QMenu("Event");
  eventMenu->addAction("Add Event", [this] { emit addEventClicked(); });
  eventMenu->addAction("Add Events", [this] { emit addEventsClicked(); });

  viewMenu->addMenu(eventMenu);

  QMenu *helpMenu = new QMenu("&Help");

  QMenuBar *menuBar = new QMenuBar();
  menuBar->addMenu(fileMenu);
  menuBar->addMenu(editMenu);
  menuBar->addMenu(viewMenu);
  menuBar->addMenu(helpMenu);
  setMenuBar(menuBar);

  StartPage *startPage = new StartPage(this);
  connect(startPage, &StartPage::newProjectClicked,
          [this] { emit newProjectClicked(); });
  connect(startPage, &StartPage::openProjectClicked,
          [this] { emit openProjectClicked(); });
  setCentralWidget(startPage);
}

void View::viewAboutProject(
    const std::unique_ptr<Data::SeismProject> &project) {

  InfoProject *infoProject = new InfoProject(project, this);
  infoProject->setModal(true);
  infoProject->show();
}

void View::updateUndoStack(QUndoStack const * const undoStack) {
    if(nullptr != _currentUndoStack) {
        disconnect(_currentUndoStack, &QUndoStack::canUndoChanged, _undoAction, &QAction::setEnabled);
        disconnect(_currentUndoStack, &QUndoStack::canRedoChanged, _redoAction, &QAction::setEnabled);
    }
    _currentUndoStack = undoStack;
    connect(_currentUndoStack, &QUndoStack::canUndoChanged, _undoAction, &QAction::setEnabled);
    connect(_currentUndoStack, &QUndoStack::canRedoChanged, _redoAction, &QAction::setEnabled);
}

void View::loadProject(Data::SeismProject const * const project) {
  delete centralWidget();
  _workPage = new WorkPage(this);
  _workPage->loadProject(project);
  setCentralWidget(_workPage);

  connect(_workPage, &WorkPage::eventSelectionChanged,
          [this](auto &select) { emit changeEventFocus(select); });
  connect(_workPage, &WorkPage::removeEventClicked,
          [this](const QUuid uuid) { emit removeEventClicked(uuid); });
  connect(_workPage, &WorkPage::viewEventClicked,
          [this](const QUuid uuid) { emit viewEventClicked(uuid); });

  emit projectPresence(true);
}

void View::addEvent(Data::SeismEvent const * const event) {
  assert(nullptr != _workPage);
  _workPage->addEvent(event);
}

void View::processedEvents(
    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &events) {
  assert(nullptr != _workPage);
  _workPage->processedEvents(events);
}

void View::updateEvent(Data::SeismEvent const * const event) {
  assert(nullptr != _workPage);
  _workPage->updateEvent(event);
}

void View::removeEvent(const QUuid &uuid) {
  assert(nullptr != _workPage);
  _workPage->removeEvent(uuid);
}

void View::addHorizon(Data::SeismHorizon const * const horizon) {
  assert(nullptr != _workPage);
  _workPage->addHorizon(horizon);
}

void View::removeHorizon(const QUuid &uuid) {
  assert(nullptr != _workPage);
  _workPage->removeHorizon(uuid);
}

void View::addWell(Data::SeismWell const * const well) {
  assert(nullptr != _workPage);
  _workPage->addWell(well);
}

void View::removeWell(const QUuid &uuid) {
  assert(nullptr != _workPage);
  _workPage->removeWell(uuid);
}

void View::addReceiver(Data::SeismReceiver const * const receiver) {
  assert(nullptr != _workPage);
  _workPage->addReceiver(receiver);
}

void View::removeReceiver(const QUuid &uuid) {
  assert(nullptr != _workPage);
  _workPage->removeReceiver(uuid);
}

void View::closeProject() {
  delete centralWidget();
  StartPage *startPage = new StartPage(this);
  connect(startPage, &StartPage::newProjectClicked,
          [this] { emit newProjectClicked(); });
  connect(startPage, &StartPage::openProjectClicked,
          [this] { emit openProjectClicked(); });
  setCentralWidget(startPage);

  emit projectPresence(false);
}

} // namespace Main
