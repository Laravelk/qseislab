#include "controller.h"

#include "data/projectsettings.h"

#include "data/seismevent.h"
#include "data/seismwell.h"
#include "event_operation/share/model.h"
#include "event_operation/share/view/3dscene/polarizationanalysiswindow.h"

#include "event_operation/share/polarizationanalysiscompute.h"
#include "event_operation/share/view/polar_graph/polargraph.h"

#include "event_operation/modification/undocommandgetter.h"

#include "data/io/segyreader.h"

#include <QMessageBox>

#include <iostream> // TODO: remove

typedef Data::IO::SegyReader SegyReader;
typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace MoreEvents {
Controller::Controller(
    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &all_events,
    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map,
    const std::list<std::shared_ptr<Data::SeismReceiver>> &receivers,
    Data::ProjectSettings *const settings, QObject *parent)
    : QObject(parent), _model(new Model(new SegyReader(), this)),
      _undoStack(std::make_unique<QUndoStack>()) {

  // prepare data for view
  std::map<QUuid, QString> wellNames_map;
  for (auto &uuid_well : wells_map) {
    wellNames_map[uuid_well.first] = uuid_well.second->getName();
  }
  std::set<QString> eventNames;
  for (auto &uuid_event : all_events) {
    eventNames.insert(uuid_event.second->getName());
  }
  _view = std::make_unique<View>(eventNames, wellNames_map, _undoStack.get());

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::infoChanged, [this] {
    if (!_currentEventUuid.isNull()) {
      _view->settingEventInfo(_events_map[_currentEventUuid].get());
    }
  });

  connect(_view.get(), &View::sendWellUuidAndFilePaths,
          [this, &receivers](auto &wellUuid, auto &filePaths) {
            for (auto &path : filePaths) {
              std::list<std::shared_ptr<Data::SeismReceiver>> receiversByWell;
              for (auto &receiver : receivers) {
                if (wellUuid == receiver->getSourseWell()->getUuid()) {
                  receiversByWell.push_back(receiver);
                }
              }

              auto components =
                  _model->getSeismComponents(receiversByWell, path);

              if (!components.empty()) {
                std::shared_ptr<SeismEvent> event =
                    std::make_shared<SeismEvent>();

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
//      std::cerr << "IN MORE EVENT CONTROLLER" << std::endl;
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
    _view->loadEvent(_events_map[_currentEventUuid].get());
  });

  connect(_view.get(), &View::hideCurrentEvent, [this]() {
    _view->unloadEvent();
    _currentEventUuid = QUuid();
  });

  connect(_view.get(), &View::removeEvent,
          [this](auto &uuid) { _events_map.erase(uuid); });

  connect(_view.get(), &View::undoClicked, this,
          &Controller::handleUndoClicked);
  connect(_view.get(), &View::redoClicked, this,
          &Controller::handleRedoClicked);

  connect(_view.get(), &View::sendPicksInfo,
          [this, settings](const auto type, const auto num, const auto l_val,
                           const auto pick_val, const auto r_val) {
            auto &event = _events_map[_currentEventUuid];
            auto &movePickParameters = settings->getMovePickParameters();
            movePickParameters.setNumber(num);
            movePickParameters.setLeftValue(l_val);
            movePickParameters.setRightValue(r_val);
            movePickParameters.setPickArrivalValue(pick_val);
            movePickParameters.setTypePick(type);

            auto command = UndoCommandGetter::get(
                Data::SeismEvent::TransformOperation::MovePick, event.get(),
                settings);
            _undoStack->push(command);
          });

  connect(_view.get(), &View::addPick,
          [this, settings](auto type, auto num, auto l_val, auto arrival,
                           auto r_val) {
            auto &event = _events_map[_currentEventUuid];

            auto &addPickParameters = settings->getAddPickParameters();
            addPickParameters.setNumber(num);
            addPickParameters.setLeftValue(l_val);
            addPickParameters.setRightValue(r_val);
            addPickParameters.setPickArrivalValue(arrival);
            addPickParameters.setTypePick(type);

            auto command = UndoCommandGetter::get(
                Data::SeismEvent::TransformOperation::AddPick, event.get(),
                settings);
            _undoStack->push(command);
          });

  connect(_view.get(), &View::removePick,
          [this, settings](const auto type, const auto num) {
            auto &event = _events_map[_currentEventUuid];

            auto &removePickParameters = settings->getRemovePickParameters();
            removePickParameters.setNum(num);
            removePickParameters.setType(type);

            auto command = UndoCommandGetter::get(
                Data::SeismEvent::TransformOperation::RemovePick, event.get(),
                settings);
            _undoStack->push(command);

            _removedPickAndNeedUpdatePolarGraph = true;

            if (_polarizationWindow) {
              _polarizationWindow->setDefault();
            }
          });

  connect(
      _view.get(), &View::eventTransformClicked, [this, settings](auto oper) {
        if (!_currentEventUuid.isNull()) {
          auto &event = _events_map[_currentEventUuid];
          auto command = UndoCommandGetter::get(oper, event.get(), settings);
          _undoStack->push(command);
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
    emit sendEventsAndStack(_events_map, _undoStack);
  }

  emit finished();
}

void Controller::handleUndoClicked() {
  auto index = _undoStack->index() - 1;
  auto command =
      static_cast<const CustomUndoCommand *>(_undoStack->command(index));

  switch (command->getType()) {
  case CustomUndoCommand::EventOperation:
    auto eventOperationCommand =
        static_cast<const EventOperationUndoCommand *>(command);

    if (eventOperationCommand->isCommon()) {
      QMessageBox *msg = new QMessageBox(QMessageBox::Warning, "Warning",
                                         "Вы пытаетесь отменить общую команду ",
                                         QMessageBox::Ok | QMessageBox::Cancel);
      auto res = msg->exec();
      if (QMessageBox::Ok == res) {
        _undoStack->undo();
      }
    } else {
      auto appliedEventUuid = *eventOperationCommand->getEventUuids().begin();
      if (_currentEventUuid != appliedEventUuid) {
        QMessageBox *msg =
            new QMessageBox(QMessageBox::Warning, "Warning",
                            "Вы пытаетесь отменить команду другого "
                            "ивента",
                            QMessageBox::Ok | QMessageBox::Cancel);
        auto res = msg->exec();
        if (QMessageBox::Cancel == res) {
          break;
        }
      }
      _undoStack->undo();
      _view->loadEvent(_events_map[appliedEventUuid].get());
    }

    break;
    //  case CustomUndoCommand::RemoveSeismObject:
    // TODO: implement!
    //    break;
  }
}

void Controller::handleRedoClicked() {
  auto index = _undoStack->index();
  auto command =
      static_cast<const CustomUndoCommand *>(_undoStack->command(index));

  switch (command->getType()) {
  case CustomUndoCommand::EventOperation:
    auto eventOperationCommand =
        static_cast<const EventOperationUndoCommand *>(command);

    if (eventOperationCommand->isCommon()) {
      QMessageBox *msg =
          new QMessageBox(QMessageBox::Warning, "Warning",
                          "Вы пытаетесь применить общую команду ",
                          QMessageBox::Ok | QMessageBox::Cancel);
      auto res = msg->exec();
      if (QMessageBox::Ok == res) {
        _undoStack->redo();
      }
    } else {
      auto appliedEventUuid = *eventOperationCommand->getEventUuids().begin();
      if (_currentEventUuid != appliedEventUuid) {
        QMessageBox *msg =
            new QMessageBox(QMessageBox::Warning, "Warning",
                            "Вы пытаетесь применить команду другого "
                            "ивента",
                            QMessageBox::Ok | QMessageBox::Cancel);
        auto res = msg->exec();
        if (QMessageBox::Cancel == res) {
          break;
        }
      }
      _undoStack->redo();
      _view->loadEvent(_events_map[appliedEventUuid].get());
    }

    break;
    //  case CustomUndoCommand::RemoveSeismObject:
    // TODO: implement!
    //    break;
  }
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

} // namespace MoreEvents
} // namespace EventOperation
