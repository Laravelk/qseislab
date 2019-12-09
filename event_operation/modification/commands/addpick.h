#pragma once
#include <QUndoCommand>

#include <memory>

#include "data/seismwavepick.h"
#include "undo_stack_work/customindividualundocommand.h"

namespace Data {
class SeismEvent;
class SeismWell;
class SeismReceiver;
class SeismWavePick;
} // namespace Data

class AddPick : public CustomIndividualUndoCommand
{
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

    explicit AddPick(const QUuid &, Data::SeismEvent *, const Parameters &);

    void undo() override;
    void redo() override;

    bool is(Data::SeismEvent::TransformOperation) const override;

private:

    Parameters _parameters;
    Data::SeismEvent *_event;
    Data::SeismWavePick _addedPick;
};
