#include "controller.h"

#include "data/seismevent.h"

#include <QFileDialog>

namespace EventOperation {
namespace SaveEvent {
Controller::Controller(QObject * parent)
    :QObject(parent)
{
}

void Controller::save(const std::set<Data::SeismEvent *> & events) {
    _events = events;

    QFileDialog *fileDialog = new QFileDialog();
    fileDialog->setFileMode(QFileDialog::Directory);

    connect(fileDialog, &QFileDialog::fileSelected, this,
            &Controller::recvDirPath);
    connect(fileDialog, &QFileDialog::finished, this, &Controller::finish);

    fileDialog->open();
}


void Controller::finish() {
    emit finished();
}

void Controller::recvDirPath(const QString &dir) {
//    SegyWriter writer;
//    for(auto event : _events) {
//        if(!writer.save(dir, event)) {
//                setNotification("Ивент с именем : " + event->getName() + " не сохранен");
//        }
//    }

    setNotification("Hello");
    finish();
}


void Controller::setNotification(const QString &text, QMessageBox::Icon icon) {
  QMessageBox *msg = new QMessageBox(icon, "Message", text, QMessageBox::Ok);
  msg->show();
}


}
}
