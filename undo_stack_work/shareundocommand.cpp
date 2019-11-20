#include "shareundocommand.h"

// ShareUndoCommand::ShareUndoCommand(const std::set<QUuid>& eventUuids,
//                                   Data::SeismEvent::TransformOperation oper,
//                                   QObject *parent)
//    : QObject(parent), _uuid(QUuid::createUuid()), _eventUuids(eventUuids),
//      _oper(oper) {}

ShareUndoCommand::ShareUndoCommand(const std::set<QUuid> &eventUuids,
                                   QObject *parent)
    : QObject(parent), _uuid(QUuid::createUuid()), _eventUuids(eventUuids) {}

const QUuid &ShareUndoCommand::getUuid() const { return _uuid; }

void ShareUndoCommand::undo() { emit applyUndo(_uuid, _eventUuids); }

void ShareUndoCommand::redo() { emit applyRedo(_uuid, _eventUuids); }
