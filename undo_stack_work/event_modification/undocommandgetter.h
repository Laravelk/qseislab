#pragma once

#include "data/seismevent.h"
//#include "event_operation/modification/testmultiplier.h"
#include "testindividualcommand.h"

class CustomIndividualUndoCommand;
using TransformOperation = Data::SeismEvent::TransformOperation;

namespace UndoCommandGetter {
template <typename... Args>
CustomIndividualUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                                 bool isShare, Data::SeismEvent *event,
                                 Args... args) {
  switch (oper) {
  case TransformOperation::TestMultiplier:
    return new TestIndividualCommand(isShare, event, args...);
    break;
  }

  return nullptr;
}
} // namespace UndoCommandGetter
