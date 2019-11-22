#include "controller.h"

#include "data/seismevent.h"
#include "data/seismwell.h"
#include "event_operation/share/model.h"
#include "event_operation/share/view/3dscene/polarizationanalysiswindow.h"
#include "event_operation/share/polarizationanalysiscompute.h"
#include "event_operation/share/view/polar_graph/polargraph.h"

#include "data/io/segyreader.h"

#include <iostream> // TODO: remove

typedef Data::IO::SegyReader SegyReader;
typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace MoreEvents {
Controller::Controller(
    const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &all_events,
    const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &wells_map,
    QObject *parent)
    : QObject(parent), _model(new Model(new SegyReader(), this)) {

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

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::sendWellUuidAndFilePaths,
          [this, &wells_map](auto &wellUuid, auto &filePaths) {
            for (auto &path : filePaths) {
              auto components =
                  _model->getSeismComponents(wells_map.at(wellUuid), path);
              if (!components.empty()) {
                std::unique_ptr<SeismEvent> event =
                    std::make_unique<SeismEvent>();
                connect(event.get(), &Data::SeismEvent::changed, []() {
                  //                            std::cout << "event changed" <<
                  //                            std::endl;
                });
                event->setName(QFileInfo(path).baseName());
                for (auto &component : components) {
                  event->addComponent(std::move(component));
                }
                auto &uuid = event->getUuid();
                _events_map[uuid] = std::move(event);
              }
            }
            _view->update(_events_map);
          });
  connect(_view.get(), &View::createPolarizationAnalysisWindow, [this]() {
    _polarizationWindow =
        new PolarizationAnalysisWindow(_events_map.at(_currentEventUuid));
    _polarizationWindow->show();
    _view->setAddPolarizationWindowButtonEnable(false);
    connect (_polarizationWindow, &QDialog::finished, [this](int status) {
        _polarizationWindow = nullptr;
        _view->setAddPolarizationWindowButtonEnable(true);
    });
  });

  connect(_view.get(), &View::calculatePolarizationAnalysisData, [this]() {
        if (_calculatePolarization == nullptr) {
            _calculatePolarization = new PolarizationAnalysisCompute(_events_map.at(_currentEventUuid));
        }
        _calculatePolarization->calculate(_events_map.at(_currentEventUuid));
        _view->getPolarGraph()->update(_events_map.at(_currentEventUuid));
        for (auto &component : _events_map[_currentEventUuid].get()->getComponents()) {
            for (auto &pick : component.get()->getWavePicks()) {
                if (pick.second.getPolarizationAnalysisData() != std::nullopt) {
                    pick.second.getPolarizationAnalysisData().value()->print();
                    std::cerr << std::endl << std::endl;
                }
            }
        }
  });

  connect(_view.get(), &View::changeCurrentEvent, [this](auto &uuid) {
    if (!_currentEventUuid.isNull()) {
      _view->settingEventInfo(_events_map[_currentEventUuid]);
    } else {
    }
    _currentEventUuid = uuid;
    _view->loadEvent(_events_map[_currentEventUuid]);
    for (auto &component : _events_map[_currentEventUuid].get()->getComponents()) {
        for (auto &pick : component.get()->getWavePicks()) {
            std::cerr << "H" << std::endl;
            if (pick.second.getPolarizationAnalysisData() != std::nullopt) {
                pick.second.getPolarizationAnalysisData().value()->print();
            }
        }
    }
  });

  connect(_view.get(), &View::hideCurrentEvent, [this]() {
    if (!_currentEventUuid.isNull()) {
      _view->settingEventInfo(_events_map[_currentEventUuid]);
    } else {
    }
    _currentEventUuid = QUuid();
    _view->unloadEvent();
  });

  connect(_view.get(), &View::removeEvent,
          [this](auto &uuid) { _events_map.erase(uuid); });

  connect(_view.get(), &View::sendPicksInfo,
          [this](const auto type, const auto num, const auto l_val,
                 const auto pick_val, const auto r_val) {
            int idx = 0;
            for (auto &component :
                 _events_map.at(_currentEventUuid)->getComponents()) {
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

  connect(_view.get(), &View::removePick, [this](const auto type, const auto num) {
      int idx = 0;
      if (_polarizationWindow) {
          _polarizationWindow->setDefault();
      }
      for (auto &component : _events_map.at(_currentEventUuid)->getComponents()) {
          if (num == idx) {
              component->removeWavePick(type);
              break;
          }
          idx++;
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
    if (!_currentEventUuid.isNull()) {
      _view->settingEventInfo(_events_map[_currentEventUuid]);
    }
    emit sendEvents(_events_map);
  }

  emit finished();
}

} // namespace MoreEvents
} // namespace EventOperation
