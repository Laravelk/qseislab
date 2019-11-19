#pragma once

#include "data/seismevent.h"

#include <QUndoStack>

class CustomIndividualUndoStack : private QUndoStack {
  Q_OBJECT
public:
  explicit CustomIndividualUndoStack(QObject *parent = nullptr);

  // TODO: implement!
  void push();

  bool tryUndo(Data::SeismEvent::TransformOperation);
  bool tryRedo(Data::SeismEvent::TransformOperation);
};
