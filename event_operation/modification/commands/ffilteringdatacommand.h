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

        int getSampleInterval() const;
        void setSampleInterval(int);

    private:
        // TODO: начальные значения какие поставить???
        int _F1;
        int _F2;
        int _F3;
        int _F4;
        int _sampleInterval;
    };

    explicit FFilteringDataCommand(const QUuid &, Data::SeismEvent *, const Parameters &);

    void undo() override;
    void redo() override;

    bool is(Data::SeismEvent::TransformOperation) const override;

private:
    Data::SeismEvent *_event;

    std::map<int, std::vector<std::vector<float>>> _oldDataMap;

    void fillOldDataList();
};

