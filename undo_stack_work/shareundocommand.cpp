#include "shareundocommand.h"

ShareUndoCommand::ShareUndoCommand(const std::set<QUuid> eventUuids,
                                   Data::SeismEvent::TransformOperation oper,
                                   QObject *parent)
    : QObject(parent), _eventUuids(eventUuids), _oper(oper) {}

void ShareUndoCommand::undo() { emit applyUndo(_eventUuids, _oper); }

void ShareUndoCommand::redo() { emit applyRedo(_eventUuids, _oper); }
