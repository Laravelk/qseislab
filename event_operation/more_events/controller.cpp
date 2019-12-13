#include "controller.h"

#include "data/projectsettings.h"

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

  connect(_view.get(), &View::updatePolarGraphSignal, [this]() {
    _view->updatePolarGraph(_events_map.at(_currentEventUuid).get());
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
    _removedPickAndNeedUpdatePolarGraph = false;
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
          [this, settings](const auto type, const auto num, const auto l_val,
                           const auto pick_val, const auto r_val) {
            auto &event = _events_map[_currentEventUuid];
            Data::ProjectSettings setting;
            MovePick::Parameters parameters;
            parameters.setNumber(num);
            parameters.setLeftValue(l_val);
            parameters.setRightValue(r_val);
            parameters.setPickArrivalValue(pick_val);
            parameters.setTypePick(type);
            setting.setMovePickParameters(parameters);
            auto command = UndoCommandGetter::get(
                Data::SeismEvent::TransformOperation::MovePick, QUuid(),
                event.get(), setting);
            _stacks_map[_currentEventUuid]->push(command);
          });

  connect(_view.get(), &View::addPick,
          [this](auto type, auto num, auto l_val, auto arrival, auto r_val) {
            auto &event = _events_map[_currentEventUuid];
            auto &component = event->getComponents()[num];
            Data::ProjectSettings setting;
            AddPick::Parameters parameters;
            parameters.setNumber(num);
            parameters.setLeftValue(l_val);
            parameters.setRightValue(r_val);
            parameters.setPickArrivalValue(arrival);
            parameters.setTypePick(type);
            setting.setAddPickParameters(parameters);
            auto command = UndoCommandGetter::get(
                Data::SeismEvent::TransformOperation::AddPick, QUuid(),
                event.get(), setting);
            _stacks_map[_currentEventUuid]->push(command);
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
            auto &event = _events_map[_currentEventUuid];
            Data::ProjectSettings setting;
            RemovePick::Parameters parameters;
            parameters.setNum(num);
            parameters.setType(type);
            setting.setRemovePickParameters(parameters);
            auto command = UndoCommandGetter::get(
                Data::SeismEvent::TransformOperation::RemovePick, QUuid(),
                event.get(), setting);
            _stacks_map[_currentEventUuid]->push(command);
            _removedPickAndNeedUpdatePolarGraph = true;

            if (_polarizationWindow) {
              _polarizationWindow->setDefault();
            }
          });

  connect(_view.get(), &View::eventTransformClicked,
          [this, &settings](auto oper) {
            if (!_currentEventUuid.isNull()) {
              auto &event = _events_map[_currentEventUuid];
              Data::ProjectSettings setting;
              FFilteringDataCommand::Parameters parameters;
              // F1 - F4?
              parameters.setF1(10);
              parameters.setF2(50);
              parameters.setF3(150);
              parameters.setF4(200);
              setting.setFFilteringParameters(parameters);
              // TEST TODO: delete
              auto command =
                  UndoCommandGetter::get(oper, QUuid(), event.get(), setting);
              _stacks_map[_currentEventUuid]->push(command);
            }
          });

  //  connect(_view.get(), &View::eventTransformSettingsClicked,
  //          [this](auto oper) { emit eventTransformSettingsClicked(oper); });

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
