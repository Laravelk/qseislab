#include "shareundocommand.h"

#include <iostream> // TODO: remove

// ShareUndoCommand::ShareUndoCommand(const std::set<QUuid>& eventUuids,
//                                   Data::SeismEvent::TransformOperation oper,
//                                   QObject *parent)
//    : QObject(parent), _uuid(QUuid::createUuid()), _eventUuids(eventUuids),
//      _oper(oper) {}

ShareUndoCommand::ShareUndoCommand(const std::set<QUuid> &eventUuids,
                                   QObject *parent)
    : QObject(parent), _uuid(QUuid::createUuid()), _eventUuids(eventUuids) {}

const QUuid &ShareUndoCommand::getUuid() const { return _uuid; }

void ShareUndoCommand::undo() {
    std::cout << "size eventUUids (before share undo) == " << _eventUuids.size() << std::endl;
    emit applyUndo(_uuid, _eventUuids);
    std::cout << "size eventUUids (after share undo) == " << _eventUuids.size() << std::endl;
}

void ShareUndoCommand::redo() { emit applyRedo(_uuid, _eventUuids); }
