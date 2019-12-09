#include "undocommandgetter.h"


namespace UndoCommandGetter {
CustomIndividualUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                                 const QUuid &shareUuid,
                                 Data::SeismEvent *event, const Data::ProjectSettings & settings) {
    switch (oper) {
    case TransformOperation::TestMultiplier:
        return new TestIndividualCommand(shareUuid, event, settings.getTestMultParameters());
        break;
    case TransformOperation::MovePick:
        return new MovePick(shareUuid, event, settings.getMovePickParameters());
        break;
    case TransformOperation::RemovePick:
        return new RemovePick(shareUuid, event, settings.getRemovePickParameters());
        break;
    case TransformOperation::AddPick:
        return new AddPick(shareUuid, event, settings.getAddPickParameters());
        break;
    }

    std::cerr << "UndoCommandGetter::get ::return nullptr" << std::endl;
    return nullptr;
}
} // namespace UndoCommandGetter
