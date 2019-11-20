#include "customindividualundostack.h"

#include "customindividualundocommand.h"

CustomIndividualUndoStack::CustomIndividualUndoStack(QObject *parent)
    : QUndoStack(parent) {}

// void CustomIndividualUndoStack::push(CustomIndividualUndoCommand *command) {
//  this->push(command);
//}

// void CustomIndividualUndoStack::undo() { this->undo(); }

// void CustomIndividualUndoStack::redo() { this->redo(); }

bool CustomIndividualUndoStack::tryUndo(const QUuid &sharedUuid) {

  auto current_command = dynamic_cast<const CustomIndividualUndoCommand *>(
      this->command(this->index()));

  if (sharedUuid == current_command->getShareUuid()) {
    this->undo();
    return true;
  } else {
    for (int i = count() - 1; i >= 0; --i) {
      auto command =
          dynamic_cast<const CustomIndividualUndoCommand *>(this->command(i));
      if (sharedUuid == command->getShareUuid()) {
        const_cast<CustomIndividualUndoCommand *>(command)->makeIndividual();
        break;
      }
    }
  }

  return false;
}

bool CustomIndividualUndoStack::tryRedo(const QUuid &sharedUuid) {

  auto current_command = dynamic_cast<const CustomIndividualUndoCommand *>(
      this->command(this->index()));

  if (sharedUuid == current_command->getShareUuid()) {
    this->redo();
    return true;
  } else {
    for (int i = count() - 1; i >= 0; --i) {
      auto command =
          dynamic_cast<const CustomIndividualUndoCommand *>(this->command(i));
      if (sharedUuid == command->getShareUuid()) {
        const_cast<CustomIndividualUndoCommand *>(command)->makeIndividual();
        break;
      }
    }
  }

  return false;
}
