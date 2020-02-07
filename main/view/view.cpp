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
View::View(QUndoStack const *const undoStack, QWidget *parent)
    : QMainWindow(parent) {
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
  auto undoAction = editMenu->addAction(QIcon(":/icons/undo.png"), "Undo",
                                        [this] { emit undoClicked(); });
  undoAction->setEnabled(undoStack->canUndo());
  connect(undoStack, &QUndoStack::canUndoChanged, undoAction,
          &QAction::setEnabled);

  auto redoAction = editMenu->addAction(QIcon(":/icons/redo.png"), "Redo",
                                        [this] { emit redoClicked(); });
  redoAction->setEnabled(undoStack->canRedo());
  connect(undoStack, &QUndoStack::canRedoChanged, redoAction,
          &QAction::setEnabled);

  editMenu->addAction("Process Events",
                      [this] { emit processEventsClicked(); });

  QMenu *parametersMenu = new QMenu("Parameters");
  parametersMenu->setIcon(QIcon(":/icons/settings.png"));

  parametersMenu->addAction(
      QIcon(":/icons/test_mult.png"), "Test Mult", [this] {
        emit eventActionSettingsClicked(
            SeismEvent::TransformOperation::TestMultiplier);
      });

  parametersMenu->addAction(QIcon(":/icons/rotate.png"), "Rotate", [this] {
    emit eventActionSettingsClicked(SeismEvent::TransformOperation::RotateData);
  });

  parametersMenu->addAction(QIcon(":/icons/ffilter.png"), "FFilter", [this] {
    emit eventActionSettingsClicked(
        SeismEvent::TransformOperation::FFilteringData);
  });

  editMenu->addMenu(parametersMenu);

  QMenu *viewMenu = new QMenu("&Data");
  viewMenu->setDisabled(true);
  connect(this, &View::projectPresence, viewMenu, &QMenu::setEnabled);

  viewMenu->addAction("Horizons", [this] { emit horizonsClicked(); });
  viewMenu->addAction("Receivers", [this] { emit receiversClicked(); });
  viewMenu->addAction("Wells", [this] { emit wellsClicked(); });

  QMenu *eventMenu = new QMenu("Event");
  //  eventMenu->addAction("Add Event", [this] { emit addEventClicked(); });
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

void View::viewAboutProject(Data::SeismProject const *const project) {
  InfoProject *infoProject = new InfoProject(project, this);
  infoProject->setModal(true);
  infoProject->show();
}

// void View::updateUndoStack(QUndoStack const *const undoStack) {
//  if (nullptr != _currentUndoStack) {
//    disconnect(_currentUndoStack, &QUndoStack::canUndoChanged, _undoAction,
//               &QAction::setEnabled);
//    disconnect(_currentUndoStack, &QUndoStack::canRedoChanged, _redoAction,
//               &QAction::setEnabled);
//  }

//  _currentUndoStack = undoStack;

//  _undoAction->setEnabled(_currentUndoStack->canUndo());
//  connect(_currentUndoStack, &QUndoStack::canUndoChanged, _undoAction,
//          &QAction::setEnabled);

//  _redoAction->setEnabled(_currentUndoStack->canRedo());
//  connect(_currentUndoStack, &QUndoStack::canRedoChanged, _redoAction,
//          &QAction::setEnabled);
//}

void View::addEventPage(QWidget *eventPage, SeismEvent const *const event) {
  assert(nullptr != _workPage);

  _workPage->addEventPage(eventPage, event);
}

void View::closeEventPage(const QUuid &uuid) {
  _workPage->closeEventPage(uuid);
}

void View::setFocusEventPage(QWidget *page) {
  _workPage->setFocusEventPage(page);
}

void View::loadProject(Data::SeismProject const *const project) {
  delete centralWidget();
  _workPage = new WorkPage(this);
  _workPage->loadProject(project);
  setCentralWidget(_workPage);

  connect(_workPage, &WorkPage::removeEventClicked,
          [this](const QUuid uuid) { emit removeEventClicked(uuid); });
  connect(_workPage, &WorkPage::viewEventClicked,
          [this](const QUuid uuid) { emit viewEventClicked(uuid); });
  connect(_workPage, &WorkPage::eventPageClosed,
          [this](auto &uuid) { emit eventPageClosed(uuid); });
  //  connect(_workPage, &WorkPage::eventSelectionChanged,
  //          [this](auto &select) { emit changeEventFocus(select); });
  connect(_workPage, &WorkPage::eventPageChanged,
          [this](auto &uuid) { emit eventPageChanged(uuid); });
  connect(_workPage, &WorkPage::eventsActionClicked,
          [this](auto &uuids, auto oper) {
            emit eventsActionClicked(uuids, oper);
          });
  connect(_workPage, &WorkPage::eventsSaveClicked, this, [this](auto& uuids) {
      emit eventsSaveClicked(uuids);
  });
  //  connect(
  //      _workPage, &WorkPage::eventActionClicked,
  //      [this](auto &uuid, auto oper) { emit eventActionClicked(uuid, oper);
  //      });

  emit projectPresence(true);
}

void View::addEvent(Data::SeismEvent const *const event) {
  assert(nullptr != _workPage);
  _workPage->addEvent(event);
}

void View::processedEvents(
    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &events) {
  assert(nullptr != _workPage);
  _workPage->processedEvents(events);
}

void View::updateEvent(Data::SeismEvent const *const event) {
  assert(nullptr != _workPage);
  _workPage->updateEvent(event);
}

void View::removeEvent(const QUuid &uuid) {
  assert(nullptr != _workPage);
  _workPage->removeEvent(uuid);
}

void View::addHorizon(Data::SeismHorizon const *const horizon) {
  assert(nullptr != _workPage);
  _workPage->addHorizon(horizon);
}

void View::removeHorizon(const QUuid &uuid) {
  assert(nullptr != _workPage);
  _workPage->removeHorizon(uuid);
}

void View::addWell(Data::SeismWell const *const well) {
  assert(nullptr != _workPage);
  _workPage->addWell(well);
}

void View::removeWell(const QUuid &uuid) {
  assert(nullptr != _workPage);
  _workPage->removeWell(uuid);
}

void View::addReceiver(Data::SeismReceiver const *const receiver) {
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

// QUuid View::getFocusEventPage() const {
//  assert(nullptr != _workPage);
//  return _workPage->getFocusEventPage();
//}

} // namespace Main
