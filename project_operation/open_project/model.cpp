#include "model.h"

#include "data/seismproject.h"

#include <QJsonDocument>

typedef Data::SeismProject SeismProject;

namespace ProjectOperation {
namespace OpenProject {
Model::Model(QObject *parent) : QObject(parent) {}

std::shared_ptr<Data::SeismProject>
Model::getSeismProjectFrom(const QString &path) {
  std::shared_ptr<SeismProject> project;

  QFile readFile(path);
  if (!readFile.open(QIODevice::ReadOnly)) {
    emit notify("Unable to open save file");
    return project;
  }
  QString val = readFile.readAll();
  readFile.close();

  QJsonDocument jsonDoc = QJsonDocument::fromJson(val.toUtf8());
  QJsonObject jsonObj = jsonDoc.object();

  try {
    project = std::make_shared<SeismProject>(jsonObj, path);
  } catch (const std::runtime_error &err) {
    project.reset();
    emit notify(err.what());
  }

  return project;
}

} // namespace OpenProject
} // namespace ProjectOperation
