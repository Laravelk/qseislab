#include "controller.h"

#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>


typedef Data::SeismProject SeismProject;


namespace ProjectOperation {
namespace SaveProject {
Controller::Controller(QObject* parent)
    :QObject(parent)
{}

void Controller::saveProject(std::unique_ptr<Data::SeismProject> project)
{
    assert(project);

    _project = std::move(project);

    if( _project->exist() ) {
        QDir dir = _project->getFileInfo().absoluteDir();
        QString dirName = dir.dirName();
        dir.cdUp();
        dir.rmdir(dirName);
        dir.mkdir(dirName);

        finish(save(_project->getFileInfo()));
        return;
    }

    saveAsProject(std::move(_project));
}

void Controller::saveAsProject(std::unique_ptr<Data::SeismProject> project)
{
    assert(project);

    _project = std::move(project);

    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setFileMode(QFileDialog::Directory);

    connect(fileDialog, SIGNAL(fileSelected(const QString& )), this, SLOT(recvFilePath(const QString& )));
    connect(fileDialog, SIGNAL(finished(int)), this, SLOT(finish(int)));
    fileDialog->open();
}

std::unique_ptr<Data::SeismProject> Controller::getProject()
{
    return std::move(_project);
}

void Controller::recvFilePath(const QString& path)
{
    assert(_project);

    QDir dir(path);
    QString projectName = _project->getName();
    dir.mkdir(projectName);
    dir.cd(projectName);


    QFileInfo fileInfo(dir, projectName + ".json");

    finish(save(fileInfo));
}

void Controller::finish(int result)
{
    if(QDialog::Rejected == result) {
        emit finished(false);
    }
    else {
        setNotification("Project is saved");
        emit finished(true);
    }
}

bool Controller::save(const QFileInfo& fileInfo)
{
    QFile saveFile( fileInfo.absoluteFilePath() );
    if ( !saveFile.open( QIODevice::WriteOnly ) ) {
        setNotification("File can not be opened");
        return false;
    }

    QJsonObject jsonObjSaving;
    try {
        _project->writeToJson(jsonObjSaving, QFileInfo(saveFile));
    } catch (std::runtime_error& err) {
        setNotification(err.what());
        return false;
    }


    QJsonDocument jsonDocSaving(jsonObjSaving);
    if( -1 == saveFile.write( jsonDocSaving.toJson() ) ) {
        saveFile.close();
        setNotification("Failed to write data to file");
        return false;
    }

    saveFile.close();

    return true;
}

void Controller::setNotification(const QString& text)
{
    QMessageBox* msg = new QMessageBox();
    msg->setWindowTitle("Message");
    msg->addButton(QMessageBox::StandardButton::Ok);
    msg->setText(text);
    msg->exec();
}


} // namespace SaveProject
} // namespace ProjectOperation
