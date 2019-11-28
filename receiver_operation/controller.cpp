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

  //  connect(_view.get(), &View::sendCsvFilePath, [this](auto &path) {
  //    // TODO: RE_SEE!

  //    auto new_wells_map = _model->getSeismReceiversFrom(path, _wells_map);
  //    if (!new_wells_map.empty()) {
  //      _wells_map = new_wells_map;
  //      for (auto &uuid_well : _wells_map) {
  //        for (auto &receiver : uuid_well.second->getReceivers()) {
  //          _view->addReceiver(receiver.get());
  //        }
  //      }
  //    }
  //  });

  connect(_view.get(), &View::sendCsvFilePath, [this](auto &path) {
    auto new_receivers =
        _model->getSeismReceiversFrom(path, _wells_map, _receivers);

    for (auto &receiver : new_receivers) {
      _receivers.push_back(receiver);
      _view->addReceiver(receiver.get());
    }
    //    if (!new_wells_map.empty()) {
    //      _wells_map = new_wells_map;
    //      for (auto &uuid_well : _wells_map) {
    //        for (auto &receiver : uuid_well.second->getReceivers()) {
    //          _view->addReceiver(receiver.get());
    //        }
    //      }
    //    }
  });

  connect(_view.get(), &View::receiverClicked, [this](auto &uuid) {
    //    for (auto &uuid_well : _wells_map) {
    //      for (auto &receiver : uuid_well.second->getReceivers()) {
    //        if (uuid == receiver->getUuid()) {
    //          _view->viewFullInfo(receiver.get());
    //        }
    //      }
    //    }
    for (auto &receiver : _receivers) {
      if (uuid == receiver->getUuid()) {
        _view->viewFullInfo(receiver.get());
      }
    }
  });

  // TODO: implement!!!!!!
  //  connect(_view.get(), &View::addReceiverClicked, [this] {
  //    auto receiver = std::make_shared<SeismReceiver>();
  //    auto wellUuid = _view->settingReceiverInfo(receiver.get());
  //    _view->addReceiver(receiver.get());

  //    _wells_map[wellUuid]->addReceiver(receiver);
  //  });

  connect(_view.get(), &View::removeReceiverClicked, [this](auto &uuid) {
    _view->removeReceiver(uuid);

    //    for (auto &uuid_well : _wells_map) {
    //      if (uuid_well.second->removeReceiver(uuid)) {
    //        break;
    //      }
    //    }
    for (auto itr = _receivers.begin(); itr != _receivers.end(); ++itr) {
      if (uuid == (*itr)->getUuid()) {
        _receivers.erase(itr);
        break;
      }
    }

    //    _receivers.erase(uuid);
  });

  connect(_view.get(), &View::removeAllReceiverClicked, [this]() {
    //    for (auto &uuid_well : _wells_map) {
    //      auto well = uuid_well.second;
    //      for (auto &receiver : well->getReceivers()) {
    //        _view->removeReceiver(receiver->getUuid());
    //      }
    //      well->removeAllReceivers();
    //    }
    for (auto &receiver : _receivers) {
      _view->removeReceiver(receiver->getUuid());
    }

    _receivers.clear();
  });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::viewReceivers(
    const std::map<QUuid, std::shared_ptr<SeismWell>> &wells_map,
    const std::list<std::shared_ptr<SeismReceiver>> &receivers) {
  _wells_map = wells_map;
  _receivers = receivers;

  //  std::map<QUuid, QString> wellNames_map;
  //  for (auto &uuid_well : _wells_map) {
  //    wellNames_map[uuid_well.first] = uuid_well.second->getName();
  //  }
  //  _view->setWellNames(wellNames_map);

  //  for (auto &uuid_well : _wells_map) {
  //    for (auto &receiver : uuid_well.second->getReceivers()) {
  //      _view->addReceiver(receiver.get());
  //    }
  //  }
  for (auto &receiver : _receivers) {
    _view->addReceiver(receiver.get());
  }

  //  _view->setModal(true); // TODO: надо ли?
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    // TODO: RE-SEE!
    //  it`s uncorrect!
    //    emit removeAllReceivers();
    //    for (auto &uuid_well : _wells_map) {
    //        for (auto &receiver : uuid_well.second->getReceivers()) {
    //            emit sendReciver(uuid_well.first, receiver);
    //        }
    //    }
    emit sendReceivers(_receivers);
  }

  emit finished();
}

} // namespace ReceiverOperation
