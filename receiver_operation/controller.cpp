#include "controller.h"

#include "data/seismreceiver.h"
#include "data/seismwell.h"
#include "model.h"

typedef Data::SeismReceiver SeismReceiver;
typedef Data::SeismWell SeismWell;

namespace ReceiverOperation {
Controller::Controller(
    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map,
    QObject *parent)
    : QObject(parent), _model(new Model(this)), _wells_map(wells_map) {

  //  for (auto &uuid_well : wells_map) {
  //    _wells_map[uuid_well.first] =
  //        std::make_unique<SeismWell>(*(uuid_well.second));
  //  }

  std::map<QUuid, QString> wellNames_map;
  for (auto &uuid_well : wells_map) {
    wellNames_map[uuid_well.first] = uuid_well.second->getName();
  }
  _view = std::make_unique<View>(wellNames_map);

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  // TODO: переделать!!!
  //  connect(_view.get(), &View::sendCsvFilePath, [this](auto &path) {
  //    auto new_wells_map = _model->getSeismReceiversFrom(path, _wells_map);
  //    if (!new_wells_map.empty()) {
  //      _wells_map = std::move(new_wells_map);
  //      for (auto &uuid_well : _wells_map) {
  //        for (auto &receiver : uuid_well.second->getReceivers()) {
  //          _view->addReceiver(receiver);
  //        }
  //      }
  //      _view->changed();
  //    }
  //  });

  connect(_view.get(), &View::receiverClicked, [this](auto uuid) {
    for (auto &uuid_well : _wells_map) {
      for (auto &receiver : uuid_well.second->getReceivers()) {
        if (uuid == receiver->getUuid()) {
          _view->viewFullInfo(receiver);
        }
      }
    }
  });

  connect(_view.get(), &View::addReceiverClicked, [this] {
    std::shared_ptr<SeismReceiver> newReceiver =
        std::make_shared<SeismReceiver>();
    const QUuid wellUuid = _view->settingReceiverInfo(newReceiver);
    _view->addReceiver(newReceiver);
    _view->changed(true);
    _wells_map[wellUuid]->addReceiver(std::move(newReceiver));
  });

  connect(_view.get(), &View::removeReceiverClicked, [this](auto uuid) {
    _view->removeReceiver(uuid);
    _view->changed(true);
    for (auto &uuid_well : _wells_map) {
      if (uuid_well.second->removeReceiver(uuid)) {
        break;
      }
    }
  });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::viewReceivers() {
  for (auto &uuid_well : _wells_map) {
    for (auto &receiver : uuid_well.second->getReceivers()) {
      _view->addReceiver(receiver);
    }
  }

  _view->setModal(true);
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    //    emit removeAllReceivers();
    //    for (auto &uuid_well : _wells_map) {
    //      for (auto &receiver : uuid_well.second->getReceivers()) {
    //        emit sendReciver(uuid_well.first, receiver);
    //      }
    //    }
    emit sendWells(_wells_map);
  }
  emit finished();
}

} // namespace ReceiverOperation
