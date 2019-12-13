#include "addpick.h"

#include "data/seismevent.h"

AddPick::AddPick(const std::set<Data::SeismEvent *> &events,
                 const AddPick::Parameters &parameters)
    : EventOperationUndoCommand(events), _parameters(parameters) {}

void AddPick::redoForOne(Data::SeismEvent *event) {
  int idx = 0;
  auto &component = event->getComponents()[_parameters.getNumber()];
  Data::SeismWavePick pick(_parameters.getTypePick(),
                           _parameters.getPickArrival());
  pick.setPolarizationLeftBorder(_parameters.getLeftValue());
  pick.setPolarizationRightBorder(_parameters.getRightValue());
  component->addWavePick(pick);

  _addedPicks[event->getUuid()] = pick;

  event->changeTrigger();
}

void AddPick::undoForOne(Data::SeismEvent *event) {
  int idx = 0;
  for (auto &component : event->getComponents()) {
    if (idx == _parameters.getNumber()) {
      component->removeWavePick(_parameters.getTypePick());
      break;
    }
    idx++;
  }
  event->changeTrigger();
}

int AddPick::Parameters::getNumber() const { return num; }

int AddPick::Parameters::getLeftValue() const { return l_val; }

int AddPick::Parameters::getPickArrival() const { return pick_arrival; }

int AddPick::Parameters::getRightValue() const { return r_val; }

Data::SeismWavePick::Type AddPick::Parameters::getTypePick() const {
  return type;
}

void AddPick::Parameters::setNumber(int number) { num = number; }

void AddPick::Parameters::setLeftValue(int val) { l_val = val; }

void AddPick::Parameters::setPickArrivalValue(int val) { pick_arrival = val; }

void AddPick::Parameters::setRightValue(int val) { r_val = val; }

void AddPick::Parameters::setTypePick(Data::SeismWavePick::Type t) { type = t; }
