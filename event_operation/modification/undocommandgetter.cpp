#include "undocommandgetter.h"

#include "data/projectsettings.h"

#include <iostream> // TODO: remove

using TransformOperation = Data::SeismEvent::TransformOperation;

namespace UndoCommandGetter {
EventOperationUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                               const std::set<Data::SeismEvent *> &events,
                               Data::ProjectSettings const *const settings) {

  switch (oper) {
  case TransformOperation::TestMultiplier:
    return new TestIndividualCommand(events, settings->getTestMultParameters());
  case TransformOperation::RotateData:
    return new RotateData(events, settings->getRotateDataParameters());
  case TransformOperation::MovePick:
    return new MovePick(events, settings->getMovePickParameters());
  case TransformOperation::RemovePick:
    return new RemovePick(events, settings->getRemovePickParameters());
  case TransformOperation::AddPick:
    return new AddPick(events, settings->getAddPickParameters());
  case TransformOperation::FFilteringData:
    return new FFilteringDataCommand(events,
                                     settings->getFFilteringParameters());
  }

  std::cerr << "UndoCommandGetter::get ::return nullptr" << std::endl;
  return nullptr;
}

EventOperationUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                               Data::SeismEvent *event,
                               Data::ProjectSettings const *const settings) {
  std::set<Data::SeismEvent *> events;
  events.insert(event);
  return get(oper, events, settings);
}

} // namespace UndoCommandGetter
