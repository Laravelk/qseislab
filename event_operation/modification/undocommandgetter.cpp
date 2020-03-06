#include "undocommandgetter.h"

#include "data/projectsettings.h"

#include <iostream> // TODO: remove

using TransformOperation = Data::SeismEvent::TransformOperation;

namespace UndoCommandGetter {
EventOperationUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                               const std::set<Data::SeismEvent *> &events,
                               Data::ProjectSettings const *const settings) {

  try {
    switch (oper) {
    case TransformOperation::TestMultiplier:
      return new TestIndividualCommand(events,
                                       settings->getTestMultParameters());
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
    case TransformOperation::ComputeAnalysis:
      return new PolarizationAnalysisCompute(
          events, settings->getPolarizationParameters());
    case TransformOperation::SetOperations:
      return new EventSetOperationsUndoCommand(
          events, settings->getSetOperationsParameters());
    }

    std::cout << "not found operation" << std::endl;
    return nullptr;
  } catch (const std::exception &) {
    std::cout << "catch exception" << std::endl;
    return nullptr;
  }
}

EventOperationUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                               Data::SeismEvent *event,
                               Data::ProjectSettings const *const settings) {
  std::set<Data::SeismEvent *> events;
  events.insert(event);
  return get(oper, events, settings);
}

} // namespace UndoCommandGetter
