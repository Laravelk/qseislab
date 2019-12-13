#pragma once

#include "data/seismevent.h"

namespace Data {
class ProjectSettings;
}
class EventOperationUndoCommand;

namespace UndoCommandGetter {

// CustomIndividualUndoCommand *get(Data::SeismEvent::TransformOperation oper,
//                                 const QUuid &shareUuid,
//                                 Data::SeismEvent *event,
//                                 const Data::ProjectSettings &settings);

EventOperationUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                               const std::set<Data::SeismEvent *> &events,
                               const Data::ProjectSettings &settings);

EventOperationUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                               Data::SeismEvent *event,
                               const Data::ProjectSettings &settings);

} // namespace UndoCommandGetter
