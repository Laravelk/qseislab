#pragma once

#include "event_operation/modification/commands/testindividualcommand.h"
#include "event_operation/modification/commands/movepick.h"

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

private:
  TestIndividualCommand::Parameters _testMultParameters;
  MovePick::Parameters _movePickParameters;
};
} // namespace Data
