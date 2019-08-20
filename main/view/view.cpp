#include "view.h"

#include "startpage.h"
#include "workpage.h"

#include "data/seismevent.h"
#include "data/seismproject.h"

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismProject SeismProject;

namespace Main {
View::View(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("MainWindow");
  setMinimumSize(850, 750);

  QAction *act;

  QMenu *fileMenu = new QMenu("&File");
  act = fileMenu->addAction("New Project", this,
                            SLOT(handleNewProjectClicked()), QKeySequence::New);

  act =
      fileMenu->addAction("Open Project", this,
                          SLOT(handleOpenProjectClicked()), QKeySequence::Open);

  act =
      fileMenu->addAction("Save Project", this,
                          SLOT(handleSaveProjectClicked()), QKeySequence::Save);
  act->setDisabled(true);
  connect(this, SIGNAL(projectPresence(bool)), act, SLOT(setEnabled(bool)));

  act = fileMenu->addAction("Close Project", this,
                            SLOT(handleCloseProjectClicked()),
                            QKeySequence::Close);
  act->setDisabled(true);
  connect(this, SIGNAL(projectPresence(bool)), act, SLOT(setEnabled(bool)));

  QMenu *editMenu = new QMenu("&Edit");
  act = editMenu->addAction("Add Event", this, SLOT(handleAddEventClicked()));
  act->setDisabled(true);
  connect(this, SIGNAL(projectPresence(bool)), act, SLOT(setEnabled(bool)));

  act = editMenu->addAction("Horizons", this, SLOT(handleHorizonsClicked()));
  act->setDisabled(true);
  connect(this, SIGNAL(projectPresence(bool)), act, SLOT(setEnabled(bool)));

  QMenu *viewMenu = new QMenu("&View");

  QMenu *helpMenu = new QMenu("&Help");

  QMenuBar *menuBar = new QMenuBar();
  menuBar->addMenu(fileMenu);
  menuBar->addMenu(editMenu);
  menuBar->addMenu(viewMenu);
  menuBar->addMenu(helpMenu);
  setMenuBar(menuBar);

  StartPage *startPage = new StartPage(this);
  connect(startPage, SIGNAL(newProjectClicked()), this,
          SLOT(handleNewProjectClicked()));
  connect(startPage, SIGNAL(openProjectClicked()), this,
          SLOT(handleOpenProjectClicked()));
  setCentralWidget(startPage);
}

void View::loadProject(const std::unique_ptr<Data::SeismProject> &project) {
  delete centralWidget();
  _workPage = new WorkPage(this);
  _workPage->loadProject(project);
  setCentralWidget(_workPage);

  //    connect(_workPage, SIGNAL(addEventClicked()), this,
  //    SLOT(handleAddEventClicked()));
  connect(_workPage, SIGNAL(removeEventClicked(const QUuid)), this,
          SLOT(handleRemoveEventClicked(const QUuid)));
  connect(_workPage, SIGNAL(viewEventClicked(const QUuid)), this,
          SLOT(handleViewEventClicked(const QUuid)));

  //    connect(_workPage, SIGNAL(addHorizonClicked()), this,
  //    SLOT(handleAddHorizonClicked()));

  //    connect(_workPage, SIGNAL(saveProjectClicked()), this,
  //    SLOT(handleSaveProjectClicked())); connect(_workPage,
  //    SIGNAL(closeProjectClicked()), this, SLOT(handleCloseProjectClicked()));

  emit projectPresence(true);
}

void View::updateProject(const std::unique_ptr<Data::SeismEvent> &event) {
  assert(nullptr != _workPage);

  _workPage->updateProject(event);
}

void View::updateProject(const std::unique_ptr<Data::SeismHorizon> &horizon) {
  assert(nullptr != _workPage);

  _workPage->updateProject(horizon);
}

void View::updateProjectRemoveEvent(const QUuid &uuid) {
  assert(nullptr != _workPage);

  _workPage->updateProjectRemoveEvent(uuid);
}

void View::updateProjectRemoveHorizon(const QUuid &uuid) {
  assert(nullptr != _workPage);

  _workPage->updateProjectRemoveHorizon(uuid);
}

void View::closeProject() {
  delete centralWidget();
  StartPage *startPage = new StartPage(this);
  connect(startPage, SIGNAL(newProjectClicked()), this,
          SLOT(handleNewProjectClicked()));
  connect(startPage, SIGNAL(openProjectClicked()), this,
          SLOT(handleOpenProjectClicked()));
  setCentralWidget(startPage);

  emit projectPresence(false);
}

void View::handleNewProjectClicked() { emit newProjectClicked(); }

void View::handleOpenProjectClicked() { emit openProjectClicked(); }

void View::handleAddEventClicked() { emit addEventClicked(); }

void View::handleViewEventClicked(const QUuid uuid) {
  emit viewEventClicked(uuid);
}

void View::handleRemoveEventClicked(const QUuid uuid) {
  emit removeEventClicked(uuid);
}

void View::handleHorizonsClicked() { emit horizonsClicked(); }

void View::handleSaveProjectClicked() { emit saveProjectClicked(); }

void View::handleCloseProjectClicked() { emit closeProjectClicked(); }

} // namespace Main
