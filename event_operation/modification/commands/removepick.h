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

class RemovePick : public CustomIndividualUndoCommand
{
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

    explicit RemovePick(const QUuid &, Data::SeismEvent *, const Parameters &);

    void undo() override;
    void redo() override;

    bool is(Data::SeismEvent::TransformOperation) const override;

private:

    Parameters _parameters;
    Data::SeismEvent *_event;
    Data::SeismWavePick _deletedPick;
};
