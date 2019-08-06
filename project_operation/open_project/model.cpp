#include "model.h"

#include "data/seismproject.h"

#include <QJsonDocument>


typedef Data::SeismProject SeismProject;


namespace ProjectOperation {
namespace OpenProject {
Model::Model(QObject* parent)
    :QObject(parent)
{}

bool Model::isValid() const
{
    return _isValid;
}

void Model::setFilePath(const QString& filePath)
{
    _isValid = true;
    _filePath = filePath;

    QFile readFile( _filePath );
    if( !readFile.open(QIODevice::ReadOnly ) ) {
        _errmsg = "Unable to open save file";
        _isValid = false;
    }
    QString val = readFile.readAll();
    readFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    try {
        _project = std::make_unique<SeismProject>(jsonObj, _filePath);
    } catch (std::runtime_error& err) {
        _project.reset();
        _errmsg = err.what();
        _isValid = false;
    }

    emit changed();
}

const QString& Model::getFilePath() const
{
    return _filePath;
}

const QString& Model::getErrMsg() const
{
    return _errmsg;
}

std::unique_ptr<Data::SeismProject>& Model::getSeismProject()
{
    return _project;
}


} // namespace OpenProject
} // namespace ProjectOperation
