#include "testindividualcommand.h"

TestIndividualCommand::TestIndividualCommand(bool isShare,
                                             Data::SeismEvent *event,
                                             float multiplier)
    : CustomIndividualUndoCommand(isShare), _event(event),
      _multiplier(multiplier) {}

void TestIndividualCommand::undo() {
  for (auto &component : _event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      float *data = trace->getBuffer().get();
      int size = trace->getBufferSize();
      for (int i = 0; i < size; ++i) {
        data[i] = data[i] / _multiplier;
      }
    }
  }
}

void TestIndividualCommand::redo() {
  for (auto &component : _event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      float *data = trace->getBuffer().get();
      int size = trace->getBufferSize();
      for (int i = 0; i < size; ++i) {
        data[i] = data[i] * _multiplier;
      }
    }
  }
}

bool TestIndividualCommand::is(
    Data::SeismEvent::TransformOperation oper) const {
  return oper == Data::SeismEvent::TransformOperation::TestMultiplier;
}
