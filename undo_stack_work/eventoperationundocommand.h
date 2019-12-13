#pragma once

#include "customundocommand.h"

#include <QUuid>

#include <set>

namespace Data {
class SeismEvent;
}

class EventOperationUndoCommand : public CustomUndoCommand {
public:
  explicit EventOperationUndoCommand(Data::SeismEvent *);

  explicit EventOperationUndoCommand(const std::set<Data::SeismEvent *> &);

  bool isIndividual() const;

  bool isCommon() const;

  const std::set<QUuid> &getEventUuids() const;

  void undo() override final;
  void redo() override final;

protected:
  virtual void redoForOne(Data::SeismEvent *) = 0;
  virtual void undoForOne(Data::SeismEvent *) = 0;

protected:
  std::set<Data::SeismEvent *> _events;

private:
  std::set<QUuid> _eventUuids;
};
