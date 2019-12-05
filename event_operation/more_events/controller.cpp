#include "controller.h"

//#include "data/projectsettings.h"

#include "data/seismevent.h"
#include "data/seismwell.h"
#include "event_operation/share/model.h"
#include "event_operation/share/view/3dscene/polarizationanalysiswindow.h"

#include "event_operation/share/polarizationanalysiscompute.h"
#include "event_operation/share/view/polar_graph/polargraph.h"

//#include "event_operation/modification/testmultiplier.h"
#include "event_operation/modification/undocommandgetter.h"

#include "data/io/segyreader.h"

#include <iostream> // TODO: remove

typedef Data::IO::SegyReader SegyReader;
typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace MoreEvents {
Controller::Controller(
    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &all_events,
    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map,
    const std::list<std::shared_ptr<Data::SeismReceiver>> &receivers,
    const Data::ProjectSettings &settings, QObject *parent)
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

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::infoChanged, [this] {
    if (!_currentEventUuid.isNull()) {
      _view->settingEventInfo(_events_map[_currentEventUuid].get());
    }
  });

  connect(
      _view.get(), &View::sendWellUuidAndFilePaths,
      [this, &receivers](auto &wellUuid, auto &filePaths) {
        for (auto &path : filePaths) {
          std::list<std::shared_ptr<Data::SeismReceiver>> receiversByWell;
          for (auto &receiver : receivers) {
            if (wellUuid == receiver->getSourseWell()->getUuid()) {
              receiversByWell.push_back(receiver);
            }
          }

          auto components = _model->getSeismComponents(receiversByWell, path);

          //              auto components =
          //                  _model->getSeismComponents(wells_map.at(wellUuid),
          //                  path);
          if (!components.empty()) {
            std::shared_ptr<SeismEvent> event = std::make_shared<SeismEvent>();

            connect(event.get(), &Data::SeismEvent::infoChanged,
                    [this](auto event) {
                      if (event->getUuid() == _currentEventUuid) {
                        _view->updateInfoEvent(event);
                      }
                    });

            connect(event.get(), &Data::SeismEvent::dataChanged,
                    [this](auto event) {
                      if (event->getUuid() == _currentEventUuid) {
                        _view->updateDataEvent(event);
                      }
                    });

            auto info = event->getInfo();
            info.setName(QFileInfo(path).baseName());
            event->setInfo(info);

            for (auto &component : components) {
              event->addComponent(std::move(component));
            }
            auto &uuid = event->getUuid();
            _events_map[uuid] = event;
            _stacks_map[uuid] = std::make_shared<CustomIndividualUndoStack>();
          }
        }
        _view->update(_events_map);
      });

  connect(_view.get(), &View::createPolarizationAnalysisWindow, [this]() {
    _polarizationWindow =
        new PolarizationAnalysisWindow(_events_map.at(_currentEventUuid));
    _polarizationWindow->show();
    _view->setAddPolarizationWindowButtonEnable(false);
    connect(_polarizationWindow, &QDialog::finished, [this](int status) {
      _polarizationWindow = nullptr;
      _view->setAddPolarizationWindowButtonEnable(true);
    });
  });

  connect(_view.get(), &View::calculatePolarizationAnalysisData, [this]() {
    if (_calculatePolarization == nullptr) {
      _calculatePolarization = new PolarizationAnalysisCompute(
          _events_map.at(_currentEventUuid).get());
    }
    _calculatePolarization->calculate();
    _view->updatePolarGraph(_events_map.at(_currentEventUuid).get());
  });

  connect(_view.get(), &View::clickOnPolarAnalysisInGraph, [this]() {
    if (!checkPolarizationAnalysisDataValid() ||
        _removedPickAndNeedUpdatePolarGraph) {
      _view.get()
          ->showWarningWindowAboutValidStatusOfPolarizationAnalysisData();
    }
  });

  connect(_view.get(), &View::changeCurrentEvent, [this](auto &uuid) {
    _currentEventUuid = uuid;
    _view->loadEvent(_events_map[_currentEventUuid].get(),
                     _stacks_map[_currentEventUuid].get());
  });

  connect(_view.get(), &View::hideCurrentEvent, [this]() {
    auto &stack = _stacks_map[_currentEventUuid];
    if (stack) {
      _view->unloadEvent(stack.get());
    }
    _currentEventUuid = QUuid();
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

                auto &picks_map = component->getWavePicks();
                auto itr_pic = picks_map.find(type);
                if (itr_pic != picks_map.end()) {
                  std::cout << "here 2" << std::endl;
                  auto &pick = itr_pic->second;
                  pick.setValidDataStatus(false);
                  pick.setArrival(pick_val);
                  pick.setPolarizationLeftBorder(l_val);
                  pick.setPolarizationRightBorder(r_val);
                } else {
                  std::cout << "here" << std::endl;
                  auto pick = Data::SeismWavePick(type, pick_val);
                  pick.setPolarizationLeftBorder(l_val);
                  pick.setPolarizationRightBorder(r_val);
                  component->addWavePick(pick);
                }
                _events_map.at(_currentEventUuid)->changeTrigger();
                break;

                //                Data::SeismWavePick wavePick =
                //                    Data::SeismWavePick(type, pick_val);

                //                // wavePick.setPolarizationLeftBorder(l_val);
                //                // wavePick.setPolarizationRightBorder(r_val);

                //                Data::SeismWavePick oldWavePick;
                //                for (auto &pick : component->getWavePicks()) {
                //                  if (type == pick.first) {
                //                    oldWavePick = pick.second;
                //                    break;
                //                  }
                //                }

                //                //                Data::SeismWavePick
                //                oldWavePick =
                //                // component->getWavePick(type);
                //                wavePick.setPolarizationAnalysisData(
                // oldWavePick.getPolarizationAnalysisData());
                //                wavePick.setPolarizationLeftBorder(l_val);
                //                wavePick.setPolarizationRightBorder(r_val);
                //                if (oldWavePick.getPolarizationLeftBorder() !=
                //                        wavePick.getPolarizationLeftBorder()
                //                        ||
                //                    oldWavePick.getPolarizationRightBorder()
                //                    !=
                //                        wavePick.getPolarizationRightBorder()
                //                        ||
                //                    oldWavePick.getArrival() !=
                //                    wavePick.getArrival()) {
                //                  wavePick.setValidDataStatus(false);
                //                }
                //                component->addWavePick(wavePick);
                //                break;
              }
              ++idx;
            }
          });

  connect(_view.get(), &View::undoClicked, [this]() {
    if (!_currentEventUuid.isNull()) {
      _stacks_map[_currentEventUuid]->undo();
    }
  });
  connect(_view.get(), &View::redoClicked, [this]() {
    if (!_currentEventUuid.isNull()) {
      _stacks_map[_currentEventUuid]->redo();
    }
  });

  connect(_view.get(), &View::removePick,
          [this](const auto type, const auto num) {
            _removedPickAndNeedUpdatePolarGraph = true;
            int idx = 0;
            if (_polarizationWindow) {
              _polarizationWindow->setDefault();
            }
            for (auto &component :
                 _events_map.at(_currentEventUuid)->getComponents()) {
              if (num == idx) {
                component->removeWavePick(type);
                break;
              }
              idx++;
            }
          });

  connect(_view.get(), &View::eventTransformClicked,
          [this, &settings](auto oper) {
            if (!_currentEventUuid.isNull()) {
              auto &event = _events_map[_currentEventUuid];

              auto command =
                  UndoCommandGetter::get(oper, QUuid(), event.get(), settings);
              _stacks_map[_currentEventUuid]->push(command);
            }
          });

  connect(_view.get(), &View::eventTransformSettingsClicked,
          [this](auto oper) { emit eventTransformSettingsClicked(oper); });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

bool Controller::checkPolarizationAnalysisDataValid() {
  for (auto &component :
       _events_map[_currentEventUuid].get()->getComponents()) {
    for (auto &pick : component->getWavePicks()) {
      if (!pick.second.getValidDataStatus()) {
        return false;
      }
    }
  }
  return true;
}

void Controller::start() {
  //  _view->setModal(true); // TODO: uncomment
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    emit sendEventsAndStacks(_events_map, _stacks_map);
  }

  emit finished();
}

} // namespace MoreEvents
} // namespace EventOperation
