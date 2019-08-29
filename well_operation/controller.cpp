#include "controller.h"

#include "data/seismproject.h"
#include "data/seismwell.h"
#include "model.h"

typedef Data::SeismWell SeismWell;
typedef Data::SeismProject SeismProject;

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
    _newWells.push_back(std::move(_tmpWell));
  });

  connect(_view.get(), &View::removeWellClicked, [this](auto uuid) {
    _view->removeWell(uuid);
    _view->changed(true);
    _removedWells.push_back(uuid);
  });

  connect(_view.get(), &View::sendFilePath, [this](auto path) {
    std::unique_ptr<SeismWell> well = _model->getSeismWellFrom(path);
    // TODO: может удалить if - ?
    if (well) {
      _tmpWell = std::move(well);
      _view->updateWell(_tmpWell);
    }
  });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::viewWells(const std::unique_ptr<SeismProject> &project) {
  for (auto &itr : project->getAllMap<SeismWell>()) {
    _view->addWell(itr.second);
  }

  _view->setModal(true);
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    for (auto &removedWell : _removedWells) {
      emit sendRemovedWell(removedWell);
    }
    for (auto &well : _newWells) {
      emit sendWell(well);
    }
  }

  emit finished();
}

} // namespace WellOperation
