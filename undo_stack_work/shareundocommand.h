#pragma once

#include "data/seismevent.h"

#include <QUndoCommand>

#include <set>

// TODO: нужно ли в этом классе хранить тип операции?

class ShareUndoCommand : public QObject, public QUndoCommand {
  Q_OBJECT
public:
  explicit ShareUndoCommand(const std::set<QUuid> &,
                            Data::SeismEvent::TransformOperation,
                            QObject *parent = nullptr);

  explicit ShareUndoCommand(const std::set<QUuid> &, QObject *parent = nullptr);

  const QUuid &getUuid() const;

  const std::set<QUuid> &getAppliedUuids() const;

  void undo() override;

  void redo() override;

signals:
  void applyUndo(const QUuid &, std::set<QUuid> &);
  void applyRedo(const QUuid &, std::set<QUuid> &);

private:
  QUuid _uuid;

  std::set<QUuid> _eventUuids;

  Data::SeismEvent::TransformOperation _oper;
};
