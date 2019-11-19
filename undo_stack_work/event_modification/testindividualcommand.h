#pragma once

#include "undo_stack_work/customindividualundocommand.h"

class TestIndividualCommand : public CustomIndividualUndoCommand {
public:
  explicit TestIndividualCommand(bool, Data::SeismEvent *, float);

  void undo() override;

  void redo() override;

  bool is(Data::SeismEvent::TransformOperation) const override;

private:
  Data::SeismEvent *_event;
  float _multiplier;
};
