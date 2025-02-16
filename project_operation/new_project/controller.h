#pragma once

#include "view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismProject;
}

namespace ProjectOperation {
namespace NewProject {
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
  std::unique_ptr<View> _view;
  std::unique_ptr<Data::SeismProject> _project;
};

} // namespace NewProject
} // namespace ProjectOperation
