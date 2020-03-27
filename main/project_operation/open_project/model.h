#pragma once

#include <QObject>

#include <memory>

namespace Data {
class SeismProject;
}

namespace ProjectOperation {
namespace OpenProject {
class Model : public QObject {
  Q_OBJECT

public:
  explicit Model(QObject *);

  std::shared_ptr<Data::SeismProject> getSeismProjectFrom(const QString &);

signals:
  void notify(const QString &) const;
};

} // namespace OpenProject
} // namespace ProjectOperation
