#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismHorizon;
} // namespace Data

namespace HorizonOperation {
class Model;
class View;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void
  viewHorizons(const std::map<QUuid, std::shared_ptr<Data::SeismHorizon>> &);

  void finish(int);

signals:
  void finished() const;

  void sendHorizons(std::map<QUuid, std::shared_ptr<Data::SeismHorizon>> &);

private:
  Model *_model;
  std::unique_ptr<View> _view;

  std::unique_ptr<Data::SeismHorizon>
      _tmpHorizon; // TODO: или все же shered_ptr

  std::map<QUuid, std::shared_ptr<Data::SeismHorizon>> _horizons_map;
};

} // namespace HorizonOperation
