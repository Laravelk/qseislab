#include "controller.h"

#include <QFileDialog>
#include <QMessageBox>


typedef Data::SeismHorizon SeismHorizon;


namespace AddHorizon {
Controller::Controller(QObject* parent)
    :QObject(parent),
     _model(new Model(this))
{
    connect(_model, SIGNAL(notify(const QString& )), this, SLOT(recvNotification(const QString& )));


    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setOption(QFileDialog::DontResolveSymlinks);
    fileDialog->setNameFilter("*.bin");

    connect(fileDialog, SIGNAL(fileSelected(const QString& )), this, SLOT(recvFilePath(const QString& )));
    connect(fileDialog, SIGNAL(finished(int)), this, SLOT(finish(int)));
    fileDialog->open();
}

void Controller::recvFilePath(const QString& path)
{
    _horizon = _model->getSeismHorizonFrom(path);
}

void Controller::recvNotification(const QString& text)
{
    QMessageBox* msg = new QMessageBox();
    msg->setWindowTitle("Message");
    msg->addButton(QMessageBox::StandardButton::Ok);
    msg->setText(text);
    msg->exec();
}

void Controller::finish(int result)
{
    if(QDialog::Accepted == result) {
        emit sendHorizon(_horizon);
    }

    emit finished();
}


} // namespace AddHorizon
