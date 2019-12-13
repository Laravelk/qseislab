#include "testindividualcommand.h"

#include "data/seismevent.h"

TestIndividualCommand::TestIndividualCommand(
    const std::set<Data::SeismEvent *> &events,
    const TestIndividualCommand::Parameters &parameters)
    : EventOperationUndoCommand(events), _multiplier(parameters.getMult()) {}

void TestIndividualCommand::redoForOne(Data::SeismEvent *event) {
  for (auto &component : event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      float *data = trace->getBuffer();
      int size = trace->getBufferSize();
      for (int i = 0; i < size; ++i) {
        data[i] = data[i] * _multiplier;
      }
    }
  }
  event->changeTrigger();
}

void TestIndividualCommand::undoForOne(Data::SeismEvent *event) {
  for (auto &component : event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      float *data = trace->getBuffer();
      int size = trace->getBufferSize();
      for (int i = 0; i < size; ++i) {
        data[i] = data[i] / _multiplier;
      }
    }
  }
  event->changeTrigger();
}

int TestIndividualCommand::Parameters::getMult() const { return _mult; }

void TestIndividualCommand::Parameters::setMult(int mult) { _mult = mult; }
