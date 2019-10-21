#pragma once

#include "view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismProject;
}

namespace ProjectOperation {
namespace OpenProject {
class Model;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void start();

  void finish(int);
signals:
  void sendProject(std::unique_ptr<Data::SeismProject> &) const;
  void finished() const;

private:
  Model *_model;

  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismProject> _project;
};

} // namespace OpenProject
} // namespace ProjectOperation
