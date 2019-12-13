#include "removepick.h"

#include "data/seismevent.h"

RemovePick::RemovePick(const std::set<Data::SeismEvent *> &events,
                       const RemovePick::Parameters &parameters)
    : EventOperationUndoCommand(events), _parameters(parameters) {}

void RemovePick::redoForOne(Data::SeismEvent *event) {
  auto &component = event->getComponents()[_parameters.getNum()];
  auto &map = component->getWavePicks();
  _deletedPicks[event->getUuid()] = map[_parameters.getType()];
  component->removeWavePick(_parameters.getType());
  event->changeTrigger();
}

void RemovePick::undoForOne(Data::SeismEvent *event) {
  auto &component = event->getComponents()[_parameters.getNum()];
  auto &pick = _deletedPicks[event->getUuid()];
  component->addWavePick(pick);
  event->changeTrigger();
}

Data::SeismWavePick::Type RemovePick::Parameters::getType() const {
  return type;
}

void RemovePick::Parameters::setType(const Data::SeismWavePick::Type &value) {
  type = value;
}

int RemovePick::Parameters::getNum() const { return num; }

void RemovePick::Parameters::setNum(int value) { num = value; }
