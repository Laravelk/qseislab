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
  act = editMenu->addAction("Undo", [this] {
    if (nullptr != _workPage) {
      emit undoClicked(_workPage->getFocusEvent());
    }
  });
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);
  act = editMenu->addAction("Redo", [this] {
    if (nullptr != _workPage) {
      emit redoClicked(_workPage->getFocusEvent());
    }
  });
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);

  act = editMenu->addAction("Add Events", [this] { emit addEventsClicked(); });
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);

  act = editMenu->addAction("Add Event", [this] { emit addEventClicked(); });
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);

  act = editMenu->addAction("Process Events",
                            [this] { emit processEventsClicked(); });
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);

  QMenu *viewMenu = new QMenu("&View");
  act = viewMenu->addAction("Horizons", [this] { emit horizonsClicked(); });
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);

  act = viewMenu->addAction("Receivers", [this] { emit receiversClicked(); });
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);

  act = viewMenu->addAction("Wells", [this] { emit wellsClicked(); });
  act->setDisabled(true);
  connect(this, &View::projectPresence, act, &QAction::setEnabled);

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

void View::updateUndoStack(const std::shared_ptr<QUndoStack> &) {}

void View::loadProject(const std::unique_ptr<Data::SeismProject> &project) {
  delete centralWidget();
  _workPage = new WorkPage(this);
  _workPage->loadProject(project);
  setCentralWidget(_workPage);

  connect(_workPage, &WorkPage::removeEventClicked,
          [this](const QUuid uuid) { emit removeEventClicked(uuid); });
  connect(_workPage, &WorkPage::viewEventClicked,
          [this](const QUuid uuid) { emit viewEventClicked(uuid); });

  emit projectPresence(true);
}

void View::addEvent(const std::shared_ptr<Data::SeismEvent> &event) {
  assert(nullptr != _workPage);
  _workPage->addEvent(event);
}

void View::processedEvents(
    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &events) {
  assert(nullptr != _workPage);
  _workPage->processedEvents(events);
}

void View::updateEvent(const std::shared_ptr<Data::SeismEvent> &event) {
  assert(nullptr != _workPage);
  _workPage->updateEvent(event);
}

void View::removeEvent(const QUuid &uuid) {
  assert(nullptr != _workPage);
  _workPage->removeEvent(uuid);
}

void View::addHorizon(const std::shared_ptr<Data::SeismHorizon> &horizon) {
  assert(nullptr != _workPage);
  _workPage->addHorizon(horizon);
}

void View::removeHorizon(const QUuid &uuid) {
  assert(nullptr != _workPage);
  _workPage->removeHorizon(uuid);
}

void View::addWell(const std::shared_ptr<Data::SeismWell> &well) {
  assert(nullptr != _workPage);
  _workPage->addWell(well);
}

void View::removeWell(const QUuid &uuid) {
  assert(nullptr != _workPage);
  _workPage->removeWell(uuid);
}

void View::addReceiver(const std::shared_ptr<Data::SeismReceiver> &receiver) {
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
