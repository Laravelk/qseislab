#include "customindividualundostack.h"

#include "customindividualundocommand.h"

CustomIndividualUndoStack::CustomIndividualUndoStack(QObject *parent)
    : QUndoStack(parent) {}

bool CustomIndividualUndoStack::tryUndo(
    Data::SeismEvent::TransformOperation oper) {

  auto current_command = dynamic_cast<const CustomIndividualUndoCommand *>(
      this->command(this->index()));

  if (current_command->isShare() && current_command->is(oper)) {
    this->undo();
    return true;
  } else {
    for (int i = count() - 1; i >= 0; --i) {
      auto command =
          dynamic_cast<const CustomIndividualUndoCommand *>(this->command(i));
      if (command->isShare() && command->is(oper)) {
        const_cast<CustomIndividualUndoCommand *>(command)->makeIndividual();
        break;
      }
    }
  }
  return false;
}

bool CustomIndividualUndoStack::tryRedo(
    Data::SeismEvent::TransformOperation oper) {

  auto current_command = dynamic_cast<const CustomIndividualUndoCommand *>(
      this->command(this->index()));

  if (current_command->isShare() && current_command->is(oper)) {
    this->redo();
    return true;
  } else {
    // TODO: здесь корректно ли идти с конца стека?

    for (int i = count() - 1; i >= 0; --i) {
      auto command =
          dynamic_cast<const CustomIndividualUndoCommand *>(this->command(i));
      if (command->isShare() && command->is(oper)) {
        const_cast<CustomIndividualUndoCommand *>(command)->makeIndividual();
        break;
      }
    }
  }
  return false;
}
