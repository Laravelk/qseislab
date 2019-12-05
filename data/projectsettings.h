#pragma once

#include "event_operation/modification/commands/testindividualcommand.h"

namespace Data {
class ProjectSettings {
public:
  explicit ProjectSettings();

  void setTestMultParameters(const TestIndividualCommand::Parameters &);
  TestIndividualCommand::Parameters &getTestMultParameters();
  const TestIndividualCommand::Parameters &getTestMultParameters() const;

private:
  TestIndividualCommand::Parameters _testMultParameters;
};
} // namespace Data
