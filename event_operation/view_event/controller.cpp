#include "controller.h"

#include "data/seismevent.h"
#include "data/seismwell.h"
#include "event_operation/share/view/3dscene/polarizationanalysiswindow.h"
#include "view.h"

#include "event_operation/modification/rotatedatatoebasis.h"
#include "event_operation/modification/testmultiplier.h"
#include "undo_stack_work/event_modification/undocommandgetter.h"

#include <QUndoStack>

#include <iostream> // TODO: remove

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace ViewEvent {
Controller::Controller(
    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &all_events,
    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map,
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

  connect(_view, &View::infoChanged,
          [this] { _view->settingEventInfo(_event.get()); });

  connect(_event.get(), &Data::SeismEvent::infoChanged,
          [this](auto event) { _view->updateInfoEvent(event); });

  connect(_event.get(), &Data::SeismEvent::dataChanged,
          [this](auto event) { _view->updateDataEvent(event); });

  //  connect(_event.get(), &Data::SeismEvent::infoChanged,
  //          [this](auto event) { std::cout << "event info changed" <<
  //          std::endl; });

  //  connect(_event.get(), &Data::SeismEvent::dataChanged,
  //          []() { std::cout << "event data changed" << std::endl; });

  connect(_view, &View::createPolarizationAnalysisWindow, [this]() {
    _polarizationWindow = new PolarizationAnalysisWindow(_event);
    _polarizationWindow->show();
  });

  connect(_view, &View::sendPicksInfo,
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
}

QWidget *Controller::getView() { return _view; }

} // namespace ViewEvent
} // namespace EventOperation
