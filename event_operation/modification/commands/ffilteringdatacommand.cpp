#include "ffilteringdatacommand.h"



FFilteringDataCommand::FFilteringDataCommand(const QUuid &shareUuid, Data::SeismEvent *event, const FFilteringDataCommand::Parameters &parameters)
    : CustomIndividualUndoCommand(shareUuid),
      _event(event)
{}

void FFilteringDataCommand::undo() {
    // TODO: implement
}

void FFilteringDataCommand::redo() {
    // TODO: implement
}

bool FFilteringDataCommand::is(Data::SeismEvent::TransformOperation oper) const {
    return oper == Data::SeismEvent::TransformOperation::FFilteringData;
}

int FFilteringDataCommand::Parameters::getF1() const {
    return _F1;
}

void FFilteringDataCommand::Parameters::setF1(int f1) {
    _F1 = f1;
}

int FFilteringDataCommand::Parameters::getF2() const {
    return _F2;
}

void FFilteringDataCommand::Parameters::setF2(int f2) {
    _F2 = f2;
}

int FFilteringDataCommand::Parameters::getF3() const {
    return _F3;
}

void FFilteringDataCommand::Parameters::setF3(int f3) {
    _F3 = f3;
}

int FFilteringDataCommand::Parameters::getF4() const {
    return _F4;
}

void FFilteringDataCommand::Parameters::setF4(int f4) {
    _F4 = f4;
}
