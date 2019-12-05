#pragma once

#include "data/projectsettings.h"

#include "commands/testindividualcommand.h"
#include "data/seismevent.h"

#include <iostream>

class CustomIndividualUndoCommand;
using TransformOperation = Data::SeismEvent::TransformOperation;

namespace UndoCommandGetter {
// template <typename... Args>
// CustomIndividualUndoCommand *get(Data::SeismEvent::TransformOperation oper,
//                                 const QUuid &shareUuid,
//                                 Data::SeismEvent *event, Args... args) {
//  switch (oper) {
//  case TransformOperation::TestMultiplier:
//    return new TestIndividualCommand(shareUuid, event, args...);
//    break;
//  }

//  std::cerr << "UndoCommandGetter::get ::return nullptr" << std::endl;
//  return nullptr;
//}

CustomIndividualUndoCommand *get(Data::SeismEvent::TransformOperation oper,
                                 const QUuid &shareUuid,
                                 Data::SeismEvent *event,
                                 const Data::ProjectSettings &settings);

// template <typename... Args>
// CustomIndividualUndoCommand *get(Data::SeismEvent::TransformOperation oper,
//                                 Data::SeismEvent *event, Args... args) {
//  return get(oper, QUuid(), event, args...);
//}

} // namespace UndoCommandGetter
