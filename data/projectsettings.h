#pragma once

#include "event_operation/modification/commands/rotatedatatoebasis.h"
#include "event_operation/modification/commands/testindividualcommand.h"

namespace Data {
class ProjectSettings {
public:
  explicit ProjectSettings();

  void setTestMultParameters(const TestIndividualCommand::Parameters &);
  TestIndividualCommand::Parameters &getTestMultParameters();
  const TestIndividualCommand::Parameters &getTestMultParameters() const;

  void setRotateDataParameters(const RotateData::Parameters &);
  RotateData::Parameters &getRotateDataParameters();
  const RotateData::Parameters &getRotateDataParameters() const;

private:
  TestIndividualCommand::Parameters _testMultParameters;

  RotateData::Parameters _rotateDataParameters;
};
} // namespace Data
