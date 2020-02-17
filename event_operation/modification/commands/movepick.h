#pragma once

#include "data/seismwavepick.h"
#include "undo_stack_work/eventoperationundocommand.h"

namespace Data {
class SeismWavePick;
} // namespace Data

class MovePick : public EventOperationUndoCommand {
public:
  class Parameters {
  public:
    int getNumber() const;
    int getLeftValue() const;
    int getPickArrival() const;
    int getRightValue() const;
    Data::SeismWavePick::Type getTypePick() const;

    void setNumber(int);
    void setLeftValue(int);
    void setPickArrivalValue(int);
    void setRightValue(int);
    void setTypePick(Data::SeismWavePick::Type);

  private:
    Data::SeismWavePick::Type type;
    int num;
    int l_val;
    int pick_arrival;
    int r_val;
  };

  explicit MovePick(const std::set<Data::SeismEvent *> &, const Parameters &);

protected:
  void redoForOne(Data::SeismEvent *) override;
  void undoForOne(Data::SeismEvent *) override;

private:
  Parameters _parameters;
  std::map<QUuid, Data::SeismWavePick> _beforeRedoPicks;
};
