#include "controller.h"

#include "data/seismwell.h"
#include "model.h"
#include "view/view.h"

typedef Data::SeismWell SeismWell;

namespace WellOperation {
Controller::Controller(QWidget *viewParent, QObject *parent)
    : QObject(parent), _model(new Model(this)), _view(new View(viewParent)) {

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view, &View::addWellClicked, [this] {
    _view->settingWellInfo(_tmpWell.get());
    _view->addWell(_tmpWell.get());

    auto &uuid = _tmpWell->getUuid();
    _wells_map[uuid] = std::move(_tmpWell);
  });

  connect(_view, &View::removeWellClicked, [this](auto &uuid) {
    _view->removeWell(uuid);

    _wells_map.erase(uuid);
  });

  connect(_view, &View::sendFilePath, [this](auto path) {
    auto well = _model->getSeismWellFrom(path);
    if (well) {
      _tmpWell = std::move(well);
      _view->loadNewWell(_tmpWell.get());
    }
  });

  connect(_view, &View::finished, this, &Controller::finish);
}

void Controller::viewWells(
    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map) {
  _wells_map = wells_map;

  for (auto &uuid_well : _wells_map) {
    _view->addWell(uuid_well.second.get());
  }

  // NOTE: _view->endOfSetup();
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {

    // TODO: сделать корректное заполнение изменившихся полей
    emit sendWells(_wells_map);
  }

  emit finished();
}

} // namespace WellOperation
