#include "removepick.h"

RemovePick::RemovePick(const QUuid &uuid, Data::SeismEvent *event, const RemovePick::Parameters &param) : CustomIndividualUndoCommand(uuid), _event(event),
    _parameters(param)
{

}

void RemovePick::undo()
{
    auto &component = _event->getComponents()[_parameters.getNum()];
    component->addWavePick(_deletedPick);
    _event->changeTrigger();
}

void RemovePick::redo()
{
    auto &component = _event->getComponents()[_parameters.getNum()];
    auto &map = component->getWavePicks();
    _deletedPick = map[_parameters.getType()];
    component->removeWavePick(_parameters.getType());
    _event->changeTrigger();
}

bool RemovePick::is(Data::SeismEvent::TransformOperation oper) const
{
   return oper == Data::SeismEvent::TransformOperation::RemovePick;
}

Data::SeismWavePick::Type RemovePick::Parameters::getType() const
{
    return type;
}

void RemovePick::Parameters::setType(const Data::SeismWavePick::Type &value)
{
    type = value;
}

int RemovePick::Parameters::getNum() const
{
    return num;
}

void RemovePick::Parameters::setNum(int value)
{
    num = value;
}
