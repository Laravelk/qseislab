#pragma once

#include "undo_stack_work/eventoperationundocommand.h"

#include <QList>
#include <vector>

class FFilteringDataCommand : public EventOperationUndoCommand {
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

  explicit FFilteringDataCommand(const std::set<Data::SeismEvent *> &,
                                 const Parameters &);

protected:
  void redoForOne(Data::SeismEvent *) override;
  void undoForOne(Data::SeismEvent *) override;

private:
  FFilteringDataCommand::Parameters _parameters;
  std::map<QUuid, std::vector<std::vector<float>>> _oldEventsTraces;

  const int MICROSECONDS_IN_SECONDS = 1000000;

  void fillOldDataList(Data::SeismEvent *);
};
