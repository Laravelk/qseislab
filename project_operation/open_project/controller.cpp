#include "controller.h"

#include "model.h"
#include "data/seismproject.h"


typedef Data::SeismProject SeismProject;


namespace ProjectOperation {
namespace OpenProject {
Controller::Controller(QObject* parent)
    :QObject(parent),
     _model(new Model(this)),
     _view(std::make_unique<View>())
{
    connect(_view.get(), SIGNAL(sendFilePath(const QString& )), this, SLOT(recvFilePath(const QString& )));
    connect(_model, SIGNAL(changed()), this, SLOT(updateProject()));
    connect(_model, SIGNAL(notify(const QString& )), this, SLOT(recvNotification(const QString& )));

    connect(_view.get(), SIGNAL(finished(int)), this, SLOT(finish(int)));

    _view->setModal(true);
    _view->show();
}

void Controller::recvFilePath(const QString& path)
{
    _model->setFilePath(path);
    if(!_model->isValid()) {
        _view->setNotification(_model->getErrMsg());
    }
}

void Controller::updateProject()
{
    _view->update(_model->getSeismProject());
}

void Controller::recvNotification(const QString& msg)
{
    _view->setNotification(msg);
}

void Controller::finish(int result)
{
    if(QDialog::Accepted == result) {
        std::unique_ptr<SeismProject> project = std::move(_model->getSeismProject());
        project->setName(_view->getName());
        project->setDate(_view->getDate());
        project->setTime(_view->getTime());
        emit sendProject(project);
    }

    emit finished();
}


} // namespace OpenProject
} // namespace ProjectOperation
