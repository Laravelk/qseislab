#include "controller.h"

#include "data/seismhorizon.h"
#include "model.h"

typedef Data::SeismHorizon SeismHorizon;

namespace HorizonOperation {

Controller::Controller(QObject *parent)
    : QObject(parent), _model(new Model(this)),
      _view(std::make_unique<View>()) {

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::addHorizonClicked, [this] {
    _view->settingHorizonInfo(_tmpHorizon.get());
    _view->addHorizon(_tmpHorizon.get());

    auto& uuid = _tmpHorizon->getUuid();
    _horizons_map[uuid] = std::move(_tmpHorizon);
  });

  connect(_view.get(), &View::removeHorizonClicked, [this](auto &uuid) {
    _view->removeHorizon(uuid);
    _horizons_map.erase(uuid);
  });

  connect(_view.get(), &View::sendFilePath, [this](auto &path) {
      auto horizon = _model->getSeismHorizonFrom(path);
    if (horizon) {
        _tmpHorizon = std::move(horizon);
      _view->updateHorizon(_tmpHorizon.get());
    }
  });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::viewHorizons(
    const std::map<QUuid, std::shared_ptr<SeismHorizon>> &horizons_map) {

  _horizons_map = horizons_map;
  for(auto& uuid_horizon : _horizons_map) {
      _view->addHorizon(uuid_horizon.second.get());
  }

  _view->setModal(true); // TODO: надо ли?
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {

    // TODO: сделать корректное заполнение изменившихся полей
    emit sendHorizons(_horizons_map);
  }

  emit finished();
}

} // namespace HorizonOperation
