#include "movepick.h"

#include "data/seismevent.h"
#include "data/seismcomponent.h"

MovePick::MovePick(const QUuid &uuid, Data::SeismEvent *event, const Parameters &parameters)
    : CustomIndividualUndoCommand(uuid), _event(event), _parameters(parameters)
{}

void MovePick::undo()
{
    int idx = 0;
    for (auto &component : _event->getComponents()) {
        if (_parameters.getNumber() == idx) {
            for (auto &picksMapElement : component->getWavePicks()) {
                if (_parameters.getTypePick() == picksMapElement.first) {
                    auto &pick = picksMapElement.second;
                    pick.setArrival(_beforeRedoPick.getArrival());
                    pick.setPolarizationLeftBorder(_beforeRedoPick.getPolarizationLeftBorder());
                    pick.setPolarizationRightBorder(_beforeRedoPick.getPolarizationRightBorder());
                }
            }
        }
        idx++;
    }
    _event->changeTrigger();
}

void MovePick::redo()
{
    int idx = 0;
    for (auto &component : _event->getComponents()) {
        if (_parameters.getNumber() == idx) {
        for (auto &picksMapElement : component->getWavePicks()) {
            if (_parameters.getTypePick() == picksMapElement.first) {
                auto &pick = picksMapElement.second;
                _beforeRedoPick = pick;
                pick.setArrival(_parameters.getPickArrival());
                pick.setPolarizationLeftBorder(_parameters.getLeftValue());
                pick.setPolarizationRightBorder(_parameters.getRightValue());
            }
        }
        }
        idx++;
    }
    _event->changeTrigger();
}

bool MovePick::is(Data::SeismEvent::TransformOperation oper) const
{
    return oper == Data::SeismEvent::TransformOperation::MovePick;
}

int MovePick::Parameters::getNumber() const
{
    return num;
}

int MovePick::Parameters::getLeftValue() const
{
    return l_val;
}

int MovePick::Parameters::getPickArrival() const
{
    return pick_arrival;
}

int MovePick::Parameters::getRightValue() const
{
    return r_val;
}

Data::SeismWavePick::Type MovePick::Parameters::getTypePick() const
{
    return type;
}

void MovePick::Parameters::setNumber(int number)
{
    num = number;
}

void MovePick::Parameters::setLeftValue(int val)
{
    l_val = val;
}

void MovePick::Parameters::setPickArrivalValue(int val)
{
    pick_arrival = val;
}

void MovePick::Parameters::setRightValue(int val)
{
    r_val = val;
}

void MovePick::Parameters::setTypePick(Data::SeismWavePick::Type t)
{
    type = t;
}
