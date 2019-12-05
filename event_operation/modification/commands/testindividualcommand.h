#pragma once

#include "undo_stack_work/customindividualundocommand.h"
#include "commandparametersinterface.h"

class TestIndividualCommand : public CustomIndividualUndoCommand {
public:
    class Parameters {
    public:
        int getMult() const;
        void setMult(int);

    private:
        int _mult = 5;
    };

  explicit TestIndividualCommand(const QUuid &, Data::SeismEvent *, const Parameters& );

  void undo() override;

  void redo() override;

  bool is(Data::SeismEvent::TransformOperation) const override;

private:
  Data::SeismEvent *_event;
  float _multiplier;
};
