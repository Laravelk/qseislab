#include "eventoperationundocommand.h"

#include "data/seismevent.h"

typedef Data::SeismEvent SeismEvent;

EventOperationUndoCommand::EventOperationUndoCommand(SeismEvent *event)
    : CustomUndoCommand(CustomUndoCommand::Type::EventOperation) {
  _events.insert(event);
  _eventUuids.insert(event->getUuid());
}

EventOperationUndoCommand::EventOperationUndoCommand(
    const std::set<SeismEvent *> &events)
    : CustomUndoCommand(CustomUndoCommand::Type::EventOperation),
      _events(events) {
  for (auto &event : _events) {
    _eventUuids.insert(event->getUuid());
  }
}

bool EventOperationUndoCommand::isIndividual() const {
  return 1 == _eventUuids.size();
}

bool EventOperationUndoCommand::isCommon() const { return !isIndividual(); }

const std::set<QUuid> &EventOperationUndoCommand::getEventUuids() const {
  return _eventUuids;
}

void EventOperationUndoCommand::undo() {
  for (auto &event : _events) {
    undoForOne(event);
  }
}

void EventOperationUndoCommand::redo() {
  for (auto &event : _events) {
    redoForOne(event);
  }
}
