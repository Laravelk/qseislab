#include "projectsettings.h"

namespace Data {
ProjectSettings::ProjectSettings() {}

TestIndividualCommand::Parameters &ProjectSettings::getTestMultParameters() {
  return _testMultParameters;
}
void ProjectSettings::setTestMultParameters(
    const TestIndividualCommand::Parameters &testMultParameters) {
  _testMultParameters = testMultParameters;
}

const TestIndividualCommand::Parameters &
ProjectSettings::getTestMultParameters() const {
  return _testMultParameters;
}

MovePick::Parameters &ProjectSettings::getMovePickParameters()
{
    return _movePickParameters;
}

const MovePick::Parameters &ProjectSettings::getMovePickParameters() const
{
    return _movePickParameters;
}

void ProjectSettings::setMovePickParameters(const MovePick::Parameters &movePickParameters)
{
    _movePickParameters = movePickParameters;
}

} // namespace Data
