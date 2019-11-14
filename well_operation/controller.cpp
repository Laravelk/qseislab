#include "controller.h"

#include "data/seismwell.h"
#include "model.h"

typedef Data::SeismWell SeismWell;

namespace WellOperation {
Controller::Controller(QObject *parent)
    : QObject(parent), _model(new Model(this)),
      _view(std::make_unique<View>()) {

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::addWellClicked, [this] {
    _view->settingWellInfo(_tmpWell);
    _view->addWell(_tmpWell);
    _view->changed(true);
    auto uuid = _tmpWell->getUuid();
    _wells[uuid] = std::move(_tmpWell);
  });

  connect(_view.get(), &View::removeWellClicked, [this](auto uuid) {
    _view->removeWell(uuid);
    _view->changed(true);
    _wells.erase(uuid);
  });

  connect(_view.get(), &View::sendFilePath, [this](auto path) {
    std::shared_ptr<SeismWell> well = _model->getSeismWellFrom(path);
    // TODO: может удалить if - ?
    if (well) {
      _tmpWell = std::move(well);
      _view->updateWell(_tmpWell);
    }
  });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::viewWells(
    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map) {
  _wells = wells_map;
  for (auto &uuid_well : _wells) {
    _view->addWell(uuid_well.second);
  }
  //  for (auto &pair : wells_map) {
  //    _wells[pair.first] = std::make_unique<Data::SeismWell>(*(pair.second));
  //    _view->addWell(_wells[pair.first]);
  //  }

  _view->setModal(true);
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    emit sendWells(_wells);
  }

  emit finished();
}

} // namespace WellOperation
