#pragma once

#include <vector>

#include "undo_stack_work/eventoperationundocommand.h"

#include <iostream> // TODO: delete
#include <initializer_list>

namespace Data {
class SeismEvent;
}

class EventSetOperationsUndoCommand : public EventOperationUndoCommand {
public:
  class Parameters {
  public:
    void addCommands(EventOperationUndoCommand &commands) {
      _commands.push_back(&commands);
    }

    void setCommands(
        std::initializer_list<EventOperationUndoCommand *> command_list) {
      for (auto &command : command_list)
        _commands.push_back(command);
    }

    const std::vector<EventOperationUndoCommand *> getCommands() const {
      return _commands;
    }

    std::vector<EventOperationUndoCommand *> getCommands() { return _commands; }

  private:
    std::vector<EventOperationUndoCommand *> _commands;
  };

  explicit EventSetOperationsUndoCommand(const std::set<Data::SeismEvent *> &,
                                         const Parameters &);

protected:
  void undoForOne(Data::SeismEvent *) override;
  void redoForOne(Data::SeismEvent *) override;

private:
  Parameters _parameters;
};
