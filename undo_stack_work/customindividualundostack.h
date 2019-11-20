#pragma once

#include "data/seismevent.h"

#include "customindividualundocommand.h"

#include <QUndoStack>

class CustomIndividualUndoStack : public QUndoStack {
  Q_OBJECT
public:
  explicit CustomIndividualUndoStack(QObject *parent = nullptr);

  // TODO: implement!
  //  void push(CustomIndividualUndoCommand *);
  //  void undo();
  //  void redo();

  bool tryUndo(const QUuid &);
  bool tryRedo(const QUuid &);
};
