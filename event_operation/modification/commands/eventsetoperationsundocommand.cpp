#include "eventsetoperationsundocommand.h"

EventSetOperationsUndoCommand::EventSetOperationsUndoCommand(const std::set<Data::SeismEvent *> &event,
                                                             const EventSetOperationsUndoCommand::Parameters &parameters) :
    EventOperationUndoCommand(event), _parameters(parameters)
{}

void EventSetOperationsUndoCommand::undoForOne(Data::SeismEvent *event)
{
    for (auto &command : _parameters.getCommands()) {
        command->undoForOne(event);
    }
}

void EventSetOperationsUndoCommand::redoForOne(Data::SeismEvent *event)
{
    for (auto &command : _parameters.getCommands()) {
        command->redoForOne(event);
    }
}
