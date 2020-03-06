#include "eventsetoperationsundocommand.h"

#include <iostream> // TODO: delete

#include "data/seismevent.h"

#include "event_operation/modification/commands/movepick.h"
#include "event_operation/modification/commands/polarizationanalysiscompute.h"

EventSetOperationsUndoCommand::EventSetOperationsUndoCommand(
    const std::set<Data::SeismEvent *> &event,
    const EventSetOperationsUndoCommand::Parameters &parameters)
    : EventOperationUndoCommand(event), _parameters(parameters) {}

void EventSetOperationsUndoCommand::undoForOne(Data::SeismEvent *event) {
  for (auto &command : _parameters.getCommands()) {
    command->undoForOne(event);
  }
}

void EventSetOperationsUndoCommand::redoForOne(Data::SeismEvent *event) {
  for (auto &command : _parameters.getCommands()) {
    command->redoForOne(event);
  }
}
