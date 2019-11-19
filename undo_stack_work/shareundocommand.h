#pragma once

#include "data/seismevent.h"

#include <QUndoCommand>

#include <set>

class ShareUndoCommand : public QObject, public QUndoCommand {
  Q_OBJECT
public:
  explicit ShareUndoCommand(const std::set<QUuid>,
                            Data::SeismEvent::TransformOperation,
                            QObject *parent = nullptr);

  void undo() override;

  void redo() override;

signals:
  void applyUndo(std::set<QUuid> &, Data::SeismEvent::TransformOperation);
  void applyRedo(std::set<QUuid> &, Data::SeismEvent::TransformOperation);

private:
  std::set<QUuid> _eventUuids;

  Data::SeismEvent::TransformOperation _oper;
};
