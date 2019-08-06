#include "controller.h"


typedef Data::SeismEvent SeismEvent;
typedef Data::SeismProject SeismProject;


using namespace ProjectOperation;


namespace Main {
Controller::Controller()
    :QObject(),
     _mainWindow(std::make_unique<View>())
{
    connect(_mainWindow.get(), SIGNAL(addEventClicked()), this, SLOT(handleAddEventClicked()));
    connect(_mainWindow.get(), SIGNAL(addHorizonClicked()), this, SLOT(handleAddHorizonClicked()));
    connect(_mainWindow.get(), SIGNAL(newProjectClicked()), this, SLOT(handleNewProjectClicked()));
    connect(_mainWindow.get(), SIGNAL(openProjectClicked()), this, SLOT(handleOpenProjectClicked()));
    connect(_mainWindow.get(), SIGNAL(saveProjectClicked()), this, SLOT(handleSaveProjectClicked()));
    connect(_mainWindow.get(), SIGNAL(closeProjectClicked()), this, SLOT(handleCloseProjectClicked()));

    _mainWindow->show();
}

void Controller::recvProject(std::unique_ptr<Data::SeismProject>& project)
{
    assert(project);

    if(_project) {
        // TODO: открытый проект нужно корректно закрыть, по необходимости сохранить
    }

    _project = std::move(project);
    connect(_project.get(), SIGNAL(changed()), this, SLOT(updateProject()));

    _mainWindow->loadProject(_project);
}

void Controller::recvEvent(std::unique_ptr<Data::SeismEvent>& event)
{
    assert(_project);
    assert(event);

    _project->addEvent(event);
}

void Controller::recvHorizon(std::unique_ptr<Data::SeismHorizon>& horizon)
{
    assert(_project);
    assert(horizon);

    _project->addHorizon(horizon);
}

void Controller::updateProject()
{
    assert(_project);

    _mainWindow->updateProject(_project);
}



void Controller::handleAddEventClicked()
{
    if(!_addEventController){
        _addEventController = std::make_unique<AddEvent::Controller>(this);
        connect(_addEventController.get(), SIGNAL(sendEvent(std::unique_ptr<Data::SeismEvent>& )), this, SLOT(recvEvent(std::unique_ptr<Data::SeismEvent>& )));
        connect(_addEventController.get(), SIGNAL(finished()), this, SLOT(deleteAddEventController()));
    }
}

void Controller::handleAddHorizonClicked()
{
    if(!_addHorizonController){
        _addHorizonController = std::make_unique<AddHorizon::Controller>(this);
        connect(_addHorizonController.get(), SIGNAL(sendHorizon(std::unique_ptr<Data::SeismHorizon>& )), this, SLOT(recvHorizon(std::unique_ptr<Data::SeismHorizon>& )));
        connect(_addHorizonController.get(), SIGNAL(finished()), this, SLOT(deleteAddHorizonController()));
    }
}

void Controller::handleCloseProjectClicked()
{
    if(!_closeProjectController){
        _closeProjectController = std::make_unique<CloseProject::Controller>(this);
        connect(_closeProjectController.get(), SIGNAL(needSaveProject()), this, SLOT(handleSaveProjectClicked()));
        connect(_closeProjectController.get(), SIGNAL(finished(bool)), this, SLOT(deleteCloseProjectController(bool)));
        connect(this, SIGNAL(savedProject(bool)), _closeProjectController.get(), SLOT(finish(bool)));

        _closeProjectController->closeProject(_project);
    }
}

void Controller::handleNewProjectClicked()
{
    if(!_newProjectController){
        _newProjectController = std::make_unique<NewProject::Controller>(this);
        connect(_newProjectController.get(), SIGNAL(sendProject(std::unique_ptr<Data::SeismProject>& )), this, SLOT(recvProject(std::unique_ptr<Data::SeismProject>& )));
        connect(_newProjectController.get(), SIGNAL(finished()), this, SLOT(deleteNewProjectController()));
    }
}

void Controller::handleOpenProjectClicked()
{
    if(!_openProjectController){
        _openProjectController = std::make_unique<OpenProject::Controller>(this);
        connect(_openProjectController.get(), SIGNAL(sendProject(std::unique_ptr<Data::SeismProject>& )), this, SLOT(recvProject(std::unique_ptr<Data::SeismProject>& )));
        connect(_openProjectController.get(), SIGNAL(finished()), this, SLOT(deleteOpenProjectController()));
    }
}

void Controller::handleSaveProjectClicked()
{
    if(!_saveProjectController){
        _saveProjectController = std::make_unique<SaveProject::Controller>(this);
        connect(_saveProjectController.get(), SIGNAL(finished(bool)), this, SLOT(deleteSaveProjectController(bool)));

        _saveProjectController->saveProject(_project);
    }
}



void Controller::deleteAddEventController()
{
    _addEventController.reset();
}

void Controller::deleteAddHorizonController()
{
    _addHorizonController.reset();
}

void Controller::deleteCloseProjectController(bool closed)
{
    _closeProjectController.reset();

    if(true == closed) {
        _mainWindow->closeProject();
        _project.reset();
    }
}

void Controller::deleteNewProjectController()
{
    _newProjectController.reset();
}

void Controller::deleteOpenProjectController()
{
    _openProjectController.reset();
}

void Controller::deleteSaveProjectController(bool saved)
{
    _project = std::move(_saveProjectController->getProject());
    _saveProjectController.reset();
    emit savedProject(saved);
}


} // namespace Main
