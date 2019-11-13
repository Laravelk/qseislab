#pragma once

#include <QUndoCommand>

namespace Data {
class SeismEvent;
class SeismWell;
class SeismReceiver;
} // namespace Data

namespace EventOperation {
namespace Modefication {
class TestMultiplier : public QUndoCommand {
public:
  TestMultiplier(Data::SeismEvent *, float);

  void undo() override;

  void redo() override;

private:
  Data::SeismEvent *_event;

  float _multiplier;
};
} // namespace Modefication
} // namespace EventOperation
