#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
// class SeismProject;
class SeismHorizon;
} // namespace Data

namespace HorizonOperation {
class Model;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  //  void viewHorizons(const std::unique_ptr<Data::SeismProject> &);
  void
  viewHorizons(const std::map<QUuid, std::shared_ptr<Data::SeismHorizon>> &);
  void finish(int);

signals:
  //  void sendHorizon(std::unique_ptr<Data::SeismHorizon> &) const;
  //  void sendRemovedHorizon(const QUuid &) const;
  void finished() const;

  void sendHorizons(std::map<QUuid, std::shared_ptr<Data::SeismHorizon>> &);

private:
  Model *_model;
  std::unique_ptr<View> _view;

  std::shared_ptr<Data::SeismHorizon> _tmpHorizon;

  //  std::vector<std::unique_ptr<Data::SeismHorizon>> _newHorizons;
  //  std::vector<QUuid> _removedHorizons;

  std::map<QUuid, std::shared_ptr<Data::SeismHorizon>> _horizons;
};

} // namespace HorizonOperation
