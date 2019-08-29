#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismProject;
class SeismHorizon;
} // namespace Data

namespace HorizonOperation {
class Model;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void viewHorizons(const std::unique_ptr<Data::SeismProject> &);
  void finish(int);

signals:
  void sendHorizon(std::unique_ptr<Data::SeismHorizon> &) const;
  void sendRemovedHorizon(const QUuid &) const;
  void finished() const;

private:
  Model *_model;
  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismHorizon> _tmpHorizon;

  std::vector<std::unique_ptr<Data::SeismHorizon>> _newHorizons;
  std::vector<QUuid> _removedHorizons;
};

} // namespace HorizonOperation
