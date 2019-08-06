#include "controller.h"

#include "model.h"


typedef Data::SeismEvent SeismEvent;


namespace AddEvent {
Controller::Controller(QObject* parent)
    :QObject(parent),
      _model(new Model(this)),
      _view(std::make_unique<View>())
{
    connect(_view.get(), SIGNAL(sendFilePath(const QString& )), this, SLOT(recvFilePath(const QString& )));
    connect(_model, SIGNAL(changed()), this, SLOT(updateEvent()));
    connect(_model, SIGNAL(notify(const QString& )), this, SLOT(recvNotification(const QString& )));
    connect(_view.get(), SIGNAL(finished(int)), this, SLOT(finish(int)));

    _view->setModal(true);
    _view->show();
}

void Controller::recvFilePath(const QString& path)
{
    _model->setFilePath(path);
}

void Controller::updateEvent() {
    _view->update(_model->getSeismEvent());
}

void Controller::recvNotification(const QString& msg)
{
    _view->setNotification(msg);
}

void Controller::finish(int result)
{
    if(QDialog::Accepted == result) {
        emit sendEvent(_model->getSeismEvent());
    }

    emit finished();
}


} // namespace AddEvent
