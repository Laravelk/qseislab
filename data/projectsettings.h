#pragma once

#include "event_operation/modification/commands/testindividualcommand.h"
#include "event_operation/modification/commands/movepick.h"
#include "event_operation/modification/commands/removepick.h"
#include "event_operation/modification/commands/addpick.h"

namespace Data {
class ProjectSettings {
public:
  explicit ProjectSettings();

  void setTestMultParameters(const TestIndividualCommand::Parameters &);
  TestIndividualCommand::Parameters &getTestMultParameters();
  const TestIndividualCommand::Parameters &getTestMultParameters() const;

  void setMovePickParameters(const MovePick::Parameters &movePickParameters);
  MovePick::Parameters &getMovePickParameters();
  const MovePick::Parameters &getMovePickParameters() const;

  void setRemovePickParameters(const RemovePick::Parameters &removePickParameters);
  RemovePick::Parameters &getRemovePickParameters();
  const RemovePick::Parameters &getRemovePickParameters() const;

  void setAddPickParameters(const AddPick::Parameters &addPickParameters);
  AddPick::Parameters &getAddPickParameters();
  const AddPick::Parameters &getAddPickParameters() const;

private:
  TestIndividualCommand::Parameters _testMultParameters;
  MovePick::Parameters _movePickParameters;
  RemovePick::Parameters _removePickParameters;
  AddPick::Parameters _addPickParameters;
};
} // namespace Data
