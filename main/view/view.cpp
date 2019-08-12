#include "view.h"

#include "startpage.h"
#include "workpage.h"

#include "data/seismevent.h"
#include "data/seismproject.h"


typedef Data::SeismEvent SeismEvent;
typedef Data::SeismProject SeismProject;


namespace Main {
View::View(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MainWindow");
    setMinimumSize(600,500);


    QMenu* fileMenu = new QMenu("File");

    QMenu* editMenu = new QMenu("Edit");

    QMenu* viewMenu = new QMenu("View");
//    QAction* newEventAct = new QAction("AddEvent");
//    newEventAct->setShortcuts(QKeySequence::New);
//    connect(newEventAct, SIGNAL(triggered()), this, SLOT(addEventAction()));
//    viewMenu->addAction(newEventAct);

    QMenu* helpMenu = new QMenu("Help");


    QMenuBar* menuBar = new QMenuBar();
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(viewMenu);
    menuBar->addMenu(helpMenu);
    setMenuBar(menuBar);


    StartPage* startPage = new StartPage(this);
    connect(startPage, SIGNAL(newProjectClicked()), this, SLOT(handleNewProjectClicked()) );
    connect(startPage, SIGNAL(openProjectClicked()), this, SLOT(handleOpenProjectClicked()) );
    setCentralWidget(startPage);
}

void View::loadProject(const std::unique_ptr<Data::SeismProject>& project)
{
    delete centralWidget();
    _workPage = new WorkPage(this);
    _workPage->loadProject(project);
    setCentralWidget(_workPage);

    connect(_workPage, SIGNAL(addEventClicked()), this, SLOT(handleAddEventClicked()));
    connect(_workPage, SIGNAL(removeEventClicked(const Data::SeismEvent::Uuid )), this, SLOT(handleRemoveEventClicked(const Data::SeismEvent::Uuid )));
    connect(_workPage, SIGNAL(viewEventClicked(const Data::SeismEvent::Uuid )), this, SLOT(handleViewEventClicked(const Data::SeismEvent::Uuid )));

    connect(_workPage, SIGNAL(addHorizonClicked()), this, SLOT(handleAddHorizonClicked()));

    connect(_workPage, SIGNAL(saveProjectClicked()), this, SLOT(handleSaveProjectClicked()));
    connect(_workPage, SIGNAL(closeProjectClicked()), this, SLOT(handleCloseProjectClicked()));
}

void View::updateProject(const std::unique_ptr<Data::SeismEvent>& event)
{
    assert(nullptr != _workPage);

    _workPage->updateProject(event);
}

void View::updateProject(const std::unique_ptr<Data::SeismHorizon>& horizon)
{
    assert(nullptr != _workPage);

    _workPage->updateProject(horizon);
}

void View::updateProjectRemoveEvent(const Data::SeismEvent::Uuid& uuid)
{
    assert(nullptr != _workPage);

    _workPage->updateProjectRemoveEvent(uuid);
}

void View::updateProjectRemoveHorizon(const Data::SeismHorizon::Uuid& uuid)
{
    assert(nullptr != _workPage);

    _workPage->updateProjectRemoveHorizon(uuid);
}

void View::closeProject()
{
    delete centralWidget();
    StartPage* startPage = new StartPage(this);
    connect(startPage, SIGNAL(newProjectClicked()), this, SLOT(handleNewProjectClicked()) );
    connect(startPage, SIGNAL(openProjectClicked()), this, SLOT(handleOpenProjectClicked()) );
    setCentralWidget(startPage);
}

void View::handleNewProjectClicked() {
    emit newProjectClicked();
}

void View::handleOpenProjectClicked() {
    emit openProjectClicked();
}

void View::handleAddEventClicked()
{
    emit addEventClicked();
}

void View::handleViewEventClicked(const SeismEvent::Uuid uuid)
{
    emit viewEventClicked(uuid);
}

void View::handleRemoveEventClicked(const SeismEvent::Uuid uuid)
{
    emit removeEventClicked(uuid);
}

void View::handleAddHorizonClicked()
{
    emit addHorizonClicked();
}

void View::handleSaveProjectClicked()
{
    emit saveProjectClicked();
}

void View::handleCloseProjectClicked()
{
    emit closeProjectClicked();
}


} // namespace Main
