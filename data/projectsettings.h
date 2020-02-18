#pragma once

#include "event_operation/modification/commands/addpick.h"
#include "event_operation/modification/commands/ffilteringdatacommand.h"
#include "event_operation/modification/commands/movepick.h"
#include "event_operation/modification/commands/removepick.h"
#include "event_operation/modification/commands/rotatedatatoebasis.h"
#include "event_operation/modification/commands/testindividualcommand.h"

namespace Data {
class ProjectSettings {
public:
  explicit ProjectSettings();

  void setTestMultParameters(const TestIndividualCommand::Parameters &);
  TestIndividualCommand::Parameters &getTestMultParameters();
  const TestIndividualCommand::Parameters &getTestMultParameters() const;

  //  void setNewTestMultParameters(const NewTestIndividualCommand::Parameters
  //  &); NewTestIndividualCommand::Parameters &getNewTestMultParameters();
  //  const NewTestIndividualCommand::Parameters &getNewTestMultParameters()
  //  const;

  void setRotateDataParameters(const RotateData::Parameters &);
  RotateData::Parameters &getRotateDataParameters();
  const RotateData::Parameters &getRotateDataParameters() const;

  void setMovePickParameters(const MovePick::Parameters &movePickParameters);
  MovePick::Parameters &getMovePickParameters();
  const MovePick::Parameters &getMovePickParameters() const;

  void
  setRemovePickParameters(const RemovePick::Parameters &removePickParameters);
  RemovePick::Parameters &getRemovePickParameters();
  const RemovePick::Parameters &getRemovePickParameters() const;

  void setAddPickParameters(const AddPick::Parameters &addPickParameters);
  AddPick::Parameters &getAddPickParameters();
  const AddPick::Parameters &getAddPickParameters() const;

  void setFFilteringParameters(const FFilteringDataCommand::Parameters &);
  FFilteringDataCommand::Parameters &getFFilteringParameters();
  const FFilteringDataCommand::Parameters &getFFilteringParameters() const;

private:
  TestIndividualCommand::Parameters _testMultParameters;

  //  NewTestIndividualCommand::Parameters _newTestMultParameters;

  RotateData::Parameters _rotateDataParameters;

  MovePick::Parameters _movePickParameters;
  RemovePick::Parameters _removePickParameters;
  AddPick::Parameters _addPickParameters;

  FFilteringDataCommand::Parameters _ffilteringParameters;
};
} // namespace Data
