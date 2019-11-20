#include "customindividualundocommand.h"

CustomIndividualUndoCommand::CustomIndividualUndoCommand(QUuid shareUuid)
    : _shareUuid(shareUuid) {}

const QUuid &CustomIndividualUndoCommand::getShareUuid() const {
  return _shareUuid;
}

void CustomIndividualUndoCommand::makeIndividual() { _shareUuid = QUuid(); }
