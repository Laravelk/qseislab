#include "controller.h"

#include "data/projectsettings.h"
#include "data/seismevent.h"
#include "data/seismwell.h"
#include "event_operation/share/view/3dscene/polarizationanalysiswindow.h"
#include "view.h"

#include "event_operation/modification/undocommandgetter.h"

#include <QUndoStack>

#include <iostream> // TODO: remove

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace ViewEvent {
Controller::Controller(
    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &all_events,
    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map,
    Data::ProjectSettings *const settings,
    const std::shared_ptr<Data::SeismEvent> &event, QObject *parent)
    : QObject(parent), _event(event) {

  // prepare data for view
  std::set<QString> eventNames;
  for (auto &uuid_event : all_events) {
    if (_event->getUuid() == uuid_event.first) {
      continue;
    }
    const auto &name = uuid_event.second->getInfo().getName();
    if (name != _event->getName()) {

      eventNames.insert(uuid_event.second->getName());
    }
  }
  _view = new View(eventNames, _event.get());

  connect(_view, &View::undoClicked,
          [this] { emit undoClicked(_event->getUuid()); });
  connect(_view, &View::redoClicked,
          [this] { emit redoClicked(_event->getUuid()); });
  connect(_view, &View::eventActionClicked, [this](auto oper) {
    emit eventActionClicked(_event->getUuid(), oper);
  });

  connect(_view, &View::infoChanged,
          [this] { _view->settingEventInfo(_event.get()); });

  connect(_event.get(), &Data::SeismEvent::infoChanged,
          [this](auto event) { _view->updateInfoEvent(event); });

  connect(_event.get(), &Data::SeismEvent::dataChanged,
          [this](auto event) { _view->updateDataEvent(event); });

  connect(_view, &View::createPolarizationAnalysisWindow, [this]() {
    _polarizationWindow = new PolarizationAnalysisWindow(_event);
    _polarizationWindow->show();
  });

  connect(_view, &View::sendPicksInfo,
          [this, settings](const auto type, const auto num, const auto l_val,
                           const auto pick_val, const auto r_val) {
            auto &movePickParameters = settings->getMovePickParameters();
            movePickParameters.setNumber(num);
            movePickParameters.setLeftValue(l_val);
            movePickParameters.setRightValue(r_val);
            movePickParameters.setPickArrivalValue(pick_val);
            movePickParameters.setTypePick(type);

            emit eventActionClicked(_event->getUuid(),
                                    SeismEvent::TransformOperation::MovePick);
          });

  connect(_view, &View::addPick,
          [this, settings](auto type, auto num, auto l_val, auto arrival,
                           auto r_val) {
            auto &addPickParameters = settings->getAddPickParameters();
            addPickParameters.setNumber(num);
            addPickParameters.setLeftValue(l_val);
            addPickParameters.setRightValue(r_val);
            addPickParameters.setPickArrivalValue(arrival);
            addPickParameters.setTypePick(type);

            emit eventActionClicked(_event->getUuid(),
                                    SeismEvent::TransformOperation::AddPick);
          });

  connect(_view, &View::removePick,
          [this, settings](const auto type, const auto num) {
            auto &removePickParameters = settings->getRemovePickParameters();
            removePickParameters.setNum(num);
            removePickParameters.setType(type);

            emit eventActionClicked(_event->getUuid(),
                                    SeismEvent::TransformOperation::RemovePick);

            //            _removedPickAndNeedUpdatePolarGraph = true; // ????

            if (_polarizationWindow) {
              _polarizationWindow->setDefault();
            }
          });
}

QWidget *Controller::getView() { return _view; }

} // namespace ViewEvent
} // namespace EventOperation
