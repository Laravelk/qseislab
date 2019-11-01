#include "controller.h"

#include "data/seismevent.h"
#include "data/seismwell.h"
#include "event_operation/share/model.h"
#include "event_operation/share/view/3dscene/polarizationanalysiswindow.h"

#include "data/io/segyreader.h"

#include <iostream> // TODO: remove

typedef Data::IO::SegyReader SegyReader;

namespace EventOperation {
namespace OneEvent {
Controller::Controller(
    const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &all_events,
    const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &wells_map,
    QObject *parent)
    : QObject(parent), _model(new Model(new SegyReader(), this)),
      _event(std::make_unique<Data::SeismEvent>()) {

  // prepare data for view
  std::map<QUuid, QString> wellNames_map;
  for (auto &uuid_well : wells_map) {
    wellNames_map[uuid_well.first] = uuid_well.second->getName();
  }
  std::set<QString> eventNames;
  for (auto &uuid_event : all_events) {
    eventNames.insert(uuid_event.second->getName());
  }
  _view = std::make_unique<View>(eventNames, wellNames_map);
  // ...

  connect(_event.get(), &Data::SeismEvent::changed,
          []() { std::cout << "event changed" << std::endl; });

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::sendWellUuidAndFilePath,
          [this, &wells_map](auto &wellUuid, auto &filePath) {
            auto components =
                _model->getSeismComponents(wells_map.at(wellUuid), filePath);
            if (!components.empty()) {
              for (auto &component : components) {
                _event->addComponent(std::move(component));
              }
              _eventNameContainer[wellUuid] = QFileInfo(filePath).baseName();
              _event->setName(generateEventName());
              _view->update(_event, wellUuid);
            }
          });
  connect(_view.get(), &View::createPolarizationAnalysisWindow, [this]() {
    _polarizationWindow = new PolarizationAnalysisWindow(_event);
    _polarizationWindow->show();
  });

  connect(_view.get(), &View::sendWellUuidForRemove,
          [this, &wells_map](auto &uuid) {
            auto &well = wells_map.at(uuid);
            for (auto &reciever : well->getReceivers()) {
              _event->removeComponentByReceiverUuid(reciever->getUuid());
            }
            _eventNameContainer.erase(uuid);
            _event->setName(generateEventName());
            _view->update(_event, uuid, well->getName());
          });

  connect(_view.get(), &View::sendPicksInfo,
          [this](const auto type, const auto num, const auto l_val,
                 const auto pick_val, const auto r_val) {
            int idx = 0;
            for (auto &component : this->_event->getComponents()) {
              if (num == idx) {
                Data::SeismWavePick wavePick =
                    Data::SeismWavePick(type, pick_val);
                wavePick.setPolarizationLeftBorder(l_val);
                wavePick.setPolarizationRightBorder(r_val);

                component->addWavePick(wavePick);
                break;
              }
              ++idx;
            }
          });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

Controller::Controller(
    const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &all_events,
    const std::unique_ptr<Data::SeismEvent> &event, QObject *parent)
    : QObject(parent), _model(nullptr),
      _event(std::make_unique<Data::SeismEvent>(*event)) {

  //    _eventNameContainer[QUuid()] = _event->getName(); // TODO: it`s OK?

  // prepare data for view
  std::set<QString> eventNames;
  for (auto &uuid_event : all_events) {
    const auto &name = uuid_event.second->getName();
    if (name != _event->getName()) {
      eventNames.insert(uuid_event.second->getName());
    }
  }
  _view = std::make_unique<View>(eventNames, _event);
  // ...

  connect(_event.get(), &Data::SeismEvent::changed,
          []() { std::cout << "event changed" << std::endl; });

  connect(_view.get(), &View::createPolarizationAnalysisWindow, [this]() {
    _polarizationWindow = new PolarizationAnalysisWindow(_event);
    _polarizationWindow->show();
  });

  connect(_view.get(), &View::sendPicksInfo,
          [this](const auto type, const auto num, const auto l_val,
                 const auto pick_val, const auto r_val) {
            int idx = 0;
            for (auto &component : this->_event->getComponents()) {
              if (num == idx) {
                Data::SeismWavePick wavePick =
                    Data::SeismWavePick(type, pick_val);
                wavePick.setPolarizationLeftBorder(l_val);
                wavePick.setPolarizationRightBorder(r_val);
                component->addWavePick(wavePick);
                break;
              }
              ++idx;
            }
          });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::start() {
  //  _view->setModal(true); // TODO: uncomment
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    _view->settingEventInfo(_event);
    emit sendEvent(_event);
  }

  emit finished();
}

QString Controller::generateEventName() const {
  QString name;
  int idx = 0;
  for (auto &uuid_word : _eventNameContainer) {
    if (0 != idx) {
      name += " --- ";
    }
    name += uuid_word.second;
    ++idx;
  }
  return name;
}

} // namespace OneEvent
} // namespace EventOperation
