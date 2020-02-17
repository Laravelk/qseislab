#pragma once

#include "undo_stack_work/eventoperationundocommand.h"

class NewTestIndividualCommand : public EventOperationUndoCommand {
public:
  class Parameters {
  public:
    int getMult() const;
    void setMult(int);

  private:
    int _mult = 5;
  };

  explicit NewTestIndividualCommand(const std::set<Data::SeismEvent *>,
                                    const Parameters &);

protected:
  void redoForOne(Data::SeismEvent *) override;
  void undoForOne(Data::SeismEvent *) override;

private:
  float _multiplier;
};
