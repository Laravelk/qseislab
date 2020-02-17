#include "movepick.h"

#include "data/seismcomponent.h"
#include "data/seismevent.h"

MovePick::MovePick(const std::set<Data::SeismEvent *> &events,
                   const Parameters &parameters)
    : EventOperationUndoCommand(events), _parameters(parameters) {}

void MovePick::redoForOne(Data::SeismEvent *event) {
  auto &component = event->getComponents()[_parameters.getNumber()];
  for (auto &picksMapElement : component->getWavePicks()) {
    if (_parameters.getTypePick() == picksMapElement.first) {
      auto &pick = picksMapElement.second;

      _beforeRedoPicks[event->getUuid()] = pick;

      pick.setArrival(_parameters.getPickArrival());
      pick.setPolarizationLeftBorder(_parameters.getLeftValue());
      pick.setPolarizationRightBorder(_parameters.getRightValue());
      pick.setValidDataStatus(false);
    }
  }
  event->changeTrigger();
}

void MovePick::undoForOne(Data::SeismEvent *event) {
  int idx = 0;
  auto &component = event->getComponents()[_parameters.getNumber()];
  for (auto &picksMapElement : component->getWavePicks()) {
    if (_parameters.getTypePick() == picksMapElement.first) {
      auto &pick = picksMapElement.second;

      auto &beforeRedoPick = _beforeRedoPicks[event->getUuid()];

      pick.setArrival(beforeRedoPick.getArrival());
      pick.setPolarizationLeftBorder(
          beforeRedoPick.getPolarizationLeftBorder());
      pick.setPolarizationRightBorder(
          beforeRedoPick.getPolarizationRightBorder());
      pick.setValidDataStatus(beforeRedoPick.getValidDataStatus());
    }
  }
  event->changeTrigger();
}

int MovePick::Parameters::getNumber() const { return num; }

int MovePick::Parameters::getLeftValue() const { return l_val; }

int MovePick::Parameters::getPickArrival() const { return pick_arrival; }

int MovePick::Parameters::getRightValue() const { return r_val; }

Data::SeismWavePick::Type MovePick::Parameters::getTypePick() const {
  return type;
}

void MovePick::Parameters::setNumber(int number) { num = number; }

void MovePick::Parameters::setLeftValue(int val) { l_val = val; }

void MovePick::Parameters::setPickArrivalValue(int val) { pick_arrival = val; }

void MovePick::Parameters::setRightValue(int val) { r_val = val; }

void MovePick::Parameters::setTypePick(Data::SeismWavePick::Type t) {
  type = t;
}
