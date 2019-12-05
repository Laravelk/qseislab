#include "testindividualcommand.h"

#include <iostream> // TODO: remove

TestIndividualCommand::TestIndividualCommand(const QUuid &shareUuid,
                                             Data::SeismEvent *event,
                                             const Parameters& parameters)
    : CustomIndividualUndoCommand(shareUuid), _event(event),
      _multiplier(parameters.getMult()) {
  std::cout << "call TestIndividualCommand constr" << std::endl;
}

void TestIndividualCommand::undo() {
  std::cout << "call TestIndividualCommand::undo" << std::endl;
  for (auto &component : _event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      float *data = trace->getBuffer();
      int size = trace->getBufferSize();
      for (int i = 0; i < size; ++i) {
        data[i] = data[i] / _multiplier;
      }
    }
  }
  _event->changeTrigger();
}

void TestIndividualCommand::redo() {
  std::cout << "call TestIndividualCommand::redo" << std::endl;
  for (auto &component : _event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      float *data = trace->getBuffer();
      int size = trace->getBufferSize();
      for (int i = 0; i < size; ++i) {
        data[i] = data[i] * _multiplier;
      }
    }
  }
  _event->changeTrigger();
}

bool TestIndividualCommand::is(
    Data::SeismEvent::TransformOperation oper) const {
  return oper == Data::SeismEvent::TransformOperation::TestMultiplier;
}

int TestIndividualCommand::Parameters::getMult() const {
    return _mult;
}

void TestIndividualCommand::Parameters::setMult(int mult) {
    _mult = mult;
}
