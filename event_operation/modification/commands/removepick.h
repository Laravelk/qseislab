#pragma once

#include "data/seismwavepick.h"
#include "undo_stack_work/eventoperationundocommand.h"

namespace Data {
class SeismWavePick;
} // namespace Data

class RemovePick : public EventOperationUndoCommand {
public:
  class Parameters {
  public:
    Data::SeismWavePick::Type getType() const;
    void setType(const Data::SeismWavePick::Type &value);

    int getNum() const;
    void setNum(int value);

  private:
    Data::SeismWavePick::Type type;
    int num;
  };

  explicit RemovePick(const std::set<Data::SeismEvent *> &, const Parameters &);

protected:
  void redoForOne(Data::SeismEvent *) override;
  void undoForOne(Data::SeismEvent *) override;

private:
  Parameters _parameters;
  std::map<QUuid, Data::SeismWavePick> _deletedPicks;
};
