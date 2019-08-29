#include "controller.h"

#include "data/seismproject.h"
#include "data/seismreceiver.h"
#include "model.h"

typedef Data::SeismReceiver SeismReceiver;
typedef Data::SeismProject SeismProject;

namespace ReceiverOperation {
Controller::Controller(QObject *parent)
    : QObject(parent), _view(std::make_unique<View>()) {

  connect(_view.get(), &View::sendCsvFilePath, [this](auto &path) {
    auto model = new Model(this);
    connect(model, &Model::notify, _view.get(), &View::setNotification);
    for (auto &receiver : model->getSeismReceiversFrom(path)) {
      _view->addReceiver(receiver);
      _newReceivers.push_back(std::move(receiver));
    }
    delete model;
  });

  connect(_view.get(), &View::addReceiverClicked, [this] {
    std::unique_ptr<SeismReceiver> newReceiver =
        std::make_unique<SeismReceiver>();
    _view->settingReceiverInfo(newReceiver);
    _view->addReceiver(newReceiver);
    _view->changed(true);
    _newReceivers.push_back(std::move(newReceiver));
  });

  connect(_view.get(), &View::removeReceiverClicked, [this](auto uuid) {
    _view->removeReceiver(uuid);
    _view->changed(true);
    _removedReceivers.push_back(uuid);
  });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::viewReceivers(const std::unique_ptr<SeismProject> &project) {
  for (auto &receiver : project->getAllList<SeismReceiver>()) {
    _view->addReceiver(receiver);
  }

  _view->setModal(true);
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    for (auto &removedReceiver : _removedReceivers) {
      emit sendRemovedReceiver(removedReceiver);
    }
    for (auto &receiver : _newReceivers) {
      emit sendReceiver(receiver);
    }
  }

  emit finished();
}

} // namespace ReceiverOperation
