#include "model.h"

#include "data/seismproject.h"

#include <QJsonDocument>

typedef Data::SeismProject SeismProject;

namespace ProjectOperation {
namespace OpenProject {
Model::Model(QObject *parent) : QObject(parent) {}

std::unique_ptr<Data::SeismProject>
Model::getSeismProjectFrom(const QString &path) {
  QFile readFile(path);
  if (!readFile.open(QIODevice::ReadOnly)) {
    emit notify("Unable to open save file");
    return std::move(_project);
  }
  QString val = readFile.readAll();
  readFile.close();

  QJsonDocument jsonDoc = QJsonDocument::fromJson(val.toUtf8());
  QJsonObject jsonObj = jsonDoc.object();

  try {
    _project = std::make_unique<SeismProject>(jsonObj, path);
  } catch (const std::runtime_error &err) {
    _project.reset();
    emit notify(err.what());
  }

  return std::move(_project);
}

} // namespace OpenProject
} // namespace ProjectOperation
