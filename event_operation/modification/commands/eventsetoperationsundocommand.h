#pragma once

#include <vector>

#include "undo_stack_work/eventoperationundocommand.h"

class EventSetOperationsUndoCommand : public EventOperationUndoCommand {
public:
  class Parameters {
  public:
    void addCommands(EventSetOperationsUndoCommand &commands) {
      _commands.push_back(&commands);
    }

    void setCommands(EventSetOperationsUndoCommand *...) {}

    const std::vector<EventSetOperationsUndoCommand *> getCommands() const {
      return _commands;
    }

    std::vector<EventSetOperationsUndoCommand *> getCommands() {
      return _commands;
    }

  private:
    std::vector<EventSetOperationsUndoCommand *> _commands;
  };

  explicit EventSetOperationsUndoCommand(const std::set<Data::SeismEvent *> &,
                                         const Parameters &);

protected:
  void undoForOne(Data::SeismEvent *) override;
  void redoForOne(Data::SeismEvent *) override;

private:
  Parameters _parameters;
};
