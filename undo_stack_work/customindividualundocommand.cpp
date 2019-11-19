#include "customindividualundocommand.h"

CustomIndividualUndoCommand::CustomIndividualUndoCommand(bool isShare)
    : _isShare(isShare) {}

bool CustomIndividualUndoCommand::isShare() const { return _isShare; }

void CustomIndividualUndoCommand::makeIndividual() { _isShare = false; }
