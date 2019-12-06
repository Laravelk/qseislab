#include "removepick.h"

RemovePick::RemovePick(const QUuid &uuid, Data::SeismEvent *event, const RemovePick::Parameters &param) : CustomIndividualUndoCommand(uuid), _event(event),
    _parameters(param)
{

}

void RemovePick::undo()
{
    int idx = 0;
    for (auto &component : _event->getComponents()) {
        if (_parameters.getNum() == idx) {
            component->addWavePick(_deletedPick);
            break;
        }
        idx++;
    }
    _event->changeTrigger();
}

void RemovePick::redo()
{
    int idx = 0;
    for (auto &component : _event->getComponents()) {
        if (_parameters.getNum() == idx) {
            for (auto &pickMapElement : component->getWavePicks()) {
                if (_parameters.getType() == pickMapElement.first) {
                    _deletedPick = pickMapElement.second;
                }
            }
            component->removeWavePick(_parameters.getType());
            break;
        }
        idx++;
    }
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
