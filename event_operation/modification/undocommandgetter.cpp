#include "undocommandgetter.h"

#include "commands/rotatedatatoebasis.h"
#include "commands/testindividualcommand.h"

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
  }

  std::cerr << "UndoCommandGetter::get ::return nullptr" << std::endl;
  return nullptr;
}
} // namespace UndoCommandGetter