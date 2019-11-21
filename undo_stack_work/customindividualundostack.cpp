#include "customindividualundostack.h"

#include "customindividualundocommand.h"

#include <iostream> // TODO: remove

CustomIndividualUndoStack::CustomIndividualUndoStack(QObject *parent)
    : QUndoStack(parent) {}

// void CustomIndividualUndoStack::push(CustomIndividualUndoCommand *command) {
//  this->push(command);
//}

// void CustomIndividualUndoStack::undo() { this->undo(); }

// void CustomIndividualUndoStack::redo() { this->redo(); }

bool CustomIndividualUndoStack::tryUndo(const QUuid &sharedUuid) {

//    std::cout << "1" << std::endl;

  auto current_command = dynamic_cast<const CustomIndividualUndoCommand *>(
      this->command(this->index()-1));

//  std::cout << "2" << std::endl;

//  if (nullptr == current_command) {
//      std::cerr << "index == " << this->index() << std::endl;
//      std::cerr << "current_command == nullptr" <<std::endl;
//      return false;
//  }

  if (sharedUuid == current_command->getShareUuid()) {
//      std::cout << "3" << std::endl;
      this->undo();
//    std::cout << "4" << std::endl;
    return true;
  } else {
    for (int i = count() - 1; i >= 0; --i) {
//        std::cout << "5 : i == " << i << std::endl;
      auto command =
          dynamic_cast<const CustomIndividualUndoCommand *>(this->command(i));
//      std::cout << "6 : i == " << i << std::endl;
      if (sharedUuid == command->getShareUuid()) {
//          std::cout << "7 : i == " << i << std::endl;
        const_cast<CustomIndividualUndoCommand *>(command)->makeIndividual();
//        std::cout << "8 : i == " << i << std::endl;
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
