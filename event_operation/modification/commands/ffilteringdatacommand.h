#pragma once

#include "undo_stack_work/customindividualundocommand.h"

#include <QList>
#include <vector>

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
    // TODO: начальные значения какие поставить??? (from SeiSee)
    int _F1 = 10;
    int _F2 = 20;
    int _F3 = 70;
    int _F4 = 1000;
  };

  explicit FFilteringDataCommand(const QUuid &, Data::SeismEvent *,
                                 const Parameters &);

  void undo() override;
  void redo() override;

  bool is(Data::SeismEvent::TransformOperation) const override;

private:
  Data::SeismEvent *_event;
  FFilteringDataCommand::Parameters _parameters;
  std::vector<std::vector<float>> _oldTraces;

  const int MICROSECONDS_IN_SECONDS = 1000000;

  void fillOldDataList();
};
