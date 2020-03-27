#include "newtestindividualcommand.h"

#include "data/seismevent.h"

NewTestIndividualCommand::NewTestIndividualCommand(
    const std::set<Data::SeismEvent *> events,
    const NewTestIndividualCommand::Parameters &parameters)
    : EventOperationUndoCommand(events), _multiplier(parameters.getMult()) {}

void NewTestIndividualCommand::redoForOne(Data::SeismEvent *event) {
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

void NewTestIndividualCommand::undoForOne(Data::SeismEvent *event) {
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

int NewTestIndividualCommand::Parameters::getMult() const { return _mult; }

void NewTestIndividualCommand::Parameters::setMult(int mult) { _mult = mult; }
