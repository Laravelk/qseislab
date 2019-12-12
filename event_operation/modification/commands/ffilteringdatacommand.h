#pragma once

#include "undo_stack_work/customindividualundocommand.h"

#include <vector>
#include <QList>

class FFilteringDataCommand : public CustomIndividualUndoCommand {
public:
    class Parameters {
    public:
        int getF1() const;
        void setF1(int);

        int getF2() const;
        void setF2(int);

        int getF3() const;
        void setF3(int);

        int getF4() const;
        void setF4(int);

    private:
        // TODO: начальные значения какие поставить???
        int _F1;
        int _F2;
        int _F3;
        int _F4;
    };

    explicit FFilteringDataCommand(const QUuid &, Data::SeismEvent *, const Parameters &);

    void undo() override;
    void redo() override;

    bool is(Data::SeismEvent::TransformOperation) const override;

private:
    Data::SeismEvent *_event;
    FFilteringDataCommand::Parameters _parameters;
    std::map<int, std::vector<std::vector<float>>> _oldDataMap;

    const int MICROSECONDS_IN_SECONDS = 1000000;

    void fillOldDataList();
};

