#pragma once

#include "data/projectsettings.h"
//#include "data/seismevent.h"

//#include <iostream>

class CustomIndividualUndoCommand;

class EventOperationUndoCommand;

namespace Data {
class SeismEvent;
}

namespace UndoCommandGetter {

CustomIndividualUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                                 const QUuid &shareUuid,
                                 Data::SeismEvent *event,
                                 const Data::ProjectSettings &settings);

EventOperationUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                               const std::set<Data::SeismEvent *> &events,
                               const Data::ProjectSettings &settings);

} // namespace UndoCommandGetter
