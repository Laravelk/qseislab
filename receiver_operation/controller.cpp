#include "controller.h"

#include "data/seismreceiver.h"
#include "data/seismwell.h"
#include "model.h"
//#include "view/view.h"

#include <iostream> // TODO: remove

typedef Data::SeismReceiver SeismReceiver;
typedef Data::SeismWell SeismWell;

namespace ReceiverOperation {
Controller::Controller(QObject *parent)
    : QObject(parent), _model(new Model(this)),
      _view(std::make_unique<View>()) {

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::sendCsvFilePath, [this](auto &path) {
    // TODO: RE_SEE!

    auto new_wells_map = _model->getSeismReceiversFrom(path, _wells_map);
    if (!new_wells_map.empty()) {
      _wells_map = new_wells_map;
      for (auto &uuid_well : _wells_map) {
        for (auto &receiver : uuid_well.second->getReceivers()) {
          _view->addReceiver(receiver.get());
        }
      }
    }
  });

  connect(_view.get(), &View::receiverClicked, [this](auto &uuid) {
    for (auto &uuid_well : _wells_map) {
      for (auto &receiver : uuid_well.second->getReceivers()) {
        if (uuid == receiver->getUuid()) {
          _view->viewFullInfo(receiver.get());
        }
      }
    }
  });

  connect(_view.get(), &View::addReceiverClicked, [this] {
    auto receiver = std::make_shared<SeismReceiver>();
    auto wellUuid = _view->settingReceiverInfo(receiver.get());
    _view->addReceiver(receiver.get());

    _wells_map[wellUuid]->addReceiver(receiver);
  });

  connect(_view.get(), &View::removeReceiverClicked, [this](auto &uuid) {
    _view->removeReceiver(uuid);

    for (auto &uuid_well : _wells_map) {
      if (uuid_well.second->removeReceiver(uuid)) {
        break;
      }
    }
  });

  connect(_view.get(), &View::removeAllReceiverClicked, [this]() {
    for (auto &uuid_well : _wells_map) {
      auto well = uuid_well.second;
      for (auto &receiver : well->getReceivers()) {
        _view->removeReceiver(receiver->getUuid());
      }
      well->removeAllReceivers();
    }
  });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::viewReceivers(
    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map) {
  _wells_map = wells_map;

  std::map<QUuid, QString> wellNames_map;
  for (auto &uuid_well : _wells_map) {
    wellNames_map[uuid_well.first] = uuid_well.second->getName();
  }
  _view->setWellNames(wellNames_map);

  for (auto &uuid_well : _wells_map) {
    for (auto &receiver : uuid_well.second->getReceivers()) {
      _view->addReceiver(receiver.get());
    }
  }

  //  _view->setModal(true); // TODO: надо ли?
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    // TODO: RE-SEE!
    //  it`s uncorrect!
    emit removeAllReceivers();
    for (auto &uuid_well : _wells_map) {
      for (auto &receiver : uuid_well.second->getReceivers()) {
        emit sendReciver(uuid_well.first, receiver);
      }
    }
  }

  emit finished();
}

} // namespace ReceiverOperation
