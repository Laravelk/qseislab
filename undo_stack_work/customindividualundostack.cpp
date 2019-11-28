#include "customindividualundostack.h"

#include "customindividualundocommand.h"

#include <iostream> // TODO: remove

CustomIndividualUndoStack::CustomIndividualUndoStack(QObject *parent)
    : QUndoStack(parent) {}

bool CustomIndividualUndoStack::tryUndo(const QUuid &sharedUuid) {

//    std::cerr << "1" << std::endl;

    int index = this->index();
  if (0 < index) {
      auto current_command = dynamic_cast<const CustomIndividualUndoCommand *>(
          this->command(index-1));
      if (sharedUuid == current_command->getShareUuid()) {
//          std::cerr << "3" << std::endl;
          this->undo();
//          std::cerr << "4" << std::endl;
          return true;
      }
  }

    for (int i = count() - 1; i >= 0; --i) {
//        std::cerr << "5 : i == " << i << std::endl;
      auto command =
          dynamic_cast<const CustomIndividualUndoCommand *>(this->command(i));
//      std::cerr << "6 : i == " << i << std::endl;
      if (sharedUuid == command->getShareUuid()) {
//          std::cerr << "7 : i == " << i << std::endl;
        const_cast<CustomIndividualUndoCommand *>(command)->makeIndividual();
//        std::cerr << "8 : i == " << i << std::endl;
        break;
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
