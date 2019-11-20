#pragma once

#include "data/seismevent.h"

#include <QUndoCommand>

class CustomIndividualUndoCommand : public QUndoCommand {
public:
  CustomIndividualUndoCommand(QUuid);

  const QUuid &getShareUuid() const;

  void makeIndividual();

  virtual bool is(Data::SeismEvent::TransformOperation) const = 0;

private:
  QUuid _shareUuid;
};
