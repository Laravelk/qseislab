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
    updateProject(project);
    setCentralWidget(_workPage);

    connect(_workPage, SIGNAL(addEventClicked()), this, SLOT(handleAddEventClicked()));
    connect(_workPage, SIGNAL(addHorizonClicked()), this, SLOT(handleAddHorizonClicked()));
    connect(_workPage, SIGNAL(saveProjectClicked()), this, SLOT(handleSaveProjectClicked()));
    connect(_workPage, SIGNAL(closeProjectClicked()), this, SLOT(handleCloseProjectClicked()));
}

void View::updateProject(const std::unique_ptr<Data::SeismProject>& project)
{
    assert(nullptr != _workPage);

    _workPage->updateProject(project);
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
