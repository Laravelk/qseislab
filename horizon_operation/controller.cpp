#include "controller.h"

#include "data/seismhorizon.h"
//#include "data/seismproject.h"
#include "model.h"

typedef Data::SeismHorizon SeismHorizon;
// typedef Data::SeismProject SeismProject;

namespace HorizonOperation {
Controller::Controller(QObject *parent)
    : QObject(parent), _model(new Model(this)),
      _view(std::make_unique<View>()) {

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::addHorizonClicked, [this] {
    _view->settingHorizonInfo(_tmpHorizon);
    _view->addHorizon(_tmpHorizon);
    _view->changed(true);
    _horizons[_tmpHorizon->getUuid()] = std::move(_tmpHorizon);
    //    _newHorizons.push_back(std::move(_tmpHorizon));
  });

  connect(_view.get(), &View::removeHorizonClicked, [this](auto uuid) {
    _view->removeHorizon(uuid);
    _view->changed(true);
    //    _removedHorizons.push_back(uuid);
    _horizons.erase(uuid);
  });

  connect(_view.get(), &View::sendFilePath, [this](auto path) {
    std::unique_ptr<SeismHorizon> horizon = _model->getSeismHorizonFrom(path);
    // TODO: может удалить if - ?
    if (horizon) {
      _tmpHorizon = std::move(horizon);
      _view->updateHorizon(_tmpHorizon);
    }
  });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::viewHorizons(
    const std::map<QUuid, std::unique_ptr<SeismHorizon>> &horizons_map) {

  for (auto &pair : horizons_map) {
    _horizons[pair.first] = std::make_unique<SeismHorizon>(*(pair.second));
    _view->addHorizon(_horizons[pair.first]);
  }

  _view->setModal(true);
  _view->show();
}

// void Controller::viewHorizons(const std::unique_ptr<SeismProject> &project) {
//  for (auto &itr : project->getAllMap<SeismHorizon>()) {
//    _view->addHorizon(itr.second);
//  }

//  _view->setModal(true);
//  _view->show();
//}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    //    for (auto &removedHorizon : _removedHorizons) {
    //      emit sendRemovedHorizon(removedHorizon);
    //    }
    //    for (auto &horizon : _newHorizons) {
    //      emit sendHorizon(horizon);
    //    }

    emit sendHorizons(_horizons);
  }

  emit finished();
}

} // namespace HorizonOperation
