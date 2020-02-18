#pragma once

#include "data/seismevent.h"

namespace Data {
class ProjectSettings;
}

class EventOperationUndoCommand;

namespace UndoCommandGetter {
EventOperationUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                               const std::set<Data::SeismEvent *> &events,
                               Data::ProjectSettings const *const settings);

EventOperationUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                               Data::SeismEvent *event,
                               Data::ProjectSettings const *const settings);
} // namespace UndoCommandGetter
