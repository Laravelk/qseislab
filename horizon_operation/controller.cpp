#include "controller.h"

#include "data/seismhorizon.h"
#include "data/seismproject.h"
#include "model.h"

typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismProject SeismProject;

namespace HorizonOperation {
Controller::Controller(QObject *parent)
    : QObject(parent), _model(new Model(this)),
      _view(std::make_unique<View>()) {

  connect(_model, SIGNAL(notify(const QString &)), this,
          SLOT(recvNotification(const QString &)));

  connect(_view.get(), SIGNAL(addHorizonClicked()), this,
          SLOT(handleAddHorizonClicked()));
  connect(_view.get(), SIGNAL(removeHorizonClicked(const QUuid)), this,
          SLOT(handleRemoveHorizonClicked(const QUuid)));
  connect(_view.get(), SIGNAL(sendFilePath(const QString &)), this,
          SLOT(recvFilePath(const QString &)));
  connect(_view.get(), SIGNAL(finished(int)), this, SLOT(finish(int)));
}

void Controller::viewHorizons(const std::unique_ptr<SeismProject> &project) {
  //  for (auto &itr : project->getHorizonsMap()) {
  for (auto &itr : project->getAllMap<SeismHorizon>()) {
    _view->addHorizon(itr.second);
  }

  _view->setModal(true);
  _view->show();
}

void Controller::handleRemoveHorizonClicked(const QUuid uuid) {
  _view->removeHorizon(uuid);
  _view->changed(true);
  _removedHorizons.push_back(uuid);
}

void Controller::handleAddHorizonClicked() {
  assert(_tmpHorizon);

  _view->settingHorizonInfo(_tmpHorizon);
  _view->addHorizon(_tmpHorizon);
  _view->changed(true);
  _newHorizons.push_back(std::move(_tmpHorizon));
}

void Controller::recvFilePath(const QString &filePath) {
  std::unique_ptr<SeismHorizon> horizon = _model->getSeismHorizonFrom(filePath);
  if (horizon) {
    _tmpHorizon = std::move(horizon);
    _view->updateHorizon(_tmpHorizon);
  }
}

void Controller::recvNotification(const QString &text) {
  _view->setNotification(text);
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    for (auto &removedHorizon : _removedHorizons) {
      emit sendRemovedHorizon(removedHorizon);
    }
    for (auto &horizon : _newHorizons) {
      emit sendHorizon(horizon);
    }
  }

  emit finished();
}

} // namespace HorizonOperation
