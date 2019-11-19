#pragma once

#include "data/seismevent.h"

#include <QUndoCommand>

class CustomIndividualUndoCommand : public QUndoCommand {
public:
  CustomIndividualUndoCommand(bool);

  bool isShare() const;
  void makeIndividual();

  virtual bool is(Data::SeismEvent::TransformOperation) const = 0;

private:
  bool _isShare;
};
