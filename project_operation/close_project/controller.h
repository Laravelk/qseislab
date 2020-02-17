#pragma once

#include <QObject>

#include <memory>

namespace Data {
class SeismProject;
}

namespace ProjectOperation {
namespace CloseProject {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void closeProject(const std::shared_ptr<Data::SeismProject> &) const;

public slots:
  void finish(bool);

signals:
  void needSaveProject() const;
  void finished(bool) const;
};

} // namespace CloseProject
} // namespace ProjectOperation
