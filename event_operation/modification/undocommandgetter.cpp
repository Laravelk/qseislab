#include "undocommandgetter.h"

#include "iostream" // TODO: remove

#include "event_operation/modification/commands/newtestindividualcommand.h"

using TransformOperation = Data::SeismEvent::TransformOperation;

namespace UndoCommandGetter {
CustomIndividualUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                                 const QUuid &shareUuid,
                                 Data::SeismEvent *event,
                                 const Data::ProjectSettings &settings) {
  switch (oper) {
  case TransformOperation::TestMultiplier:
    return new TestIndividualCommand(shareUuid, event,
                                     settings.getTestMultParameters());
  case TransformOperation::RotateData:
    return new RotateData(shareUuid, event, settings.getRotateDataParameters());
  case TransformOperation::MovePick:
    return new MovePick(shareUuid, event, settings.getMovePickParameters());
  case TransformOperation::RemovePick:
    return new RemovePick(shareUuid, event, settings.getRemovePickParameters());
  case TransformOperation::AddPick:
    return new AddPick(shareUuid, event, settings.getAddPickParameters());
  case TransformOperation::FFilteringData:
    return new FFilteringDataCommand(shareUuid, event,
                                     settings.getFFilteringParameters());
  }

  std::cerr << "UndoCommandGetter::get ::return nullptr" << std::endl;
  return nullptr;
}

EventOperationUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                               const std::set<Data::SeismEvent *> &events,
                               const Data::ProjectSettings &settings) {

  switch (oper) {
  case TransformOperation::TestMultiplier:
    return new NewTestIndividualCommand(events,
                                        settings.getNewTestMultParameters());
  }

  std::cerr << "UndoCommandGetter::get(for new) ::return nullptr" << std::endl;
  return nullptr;
}
} // namespace UndoCommandGetter
