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

void ProjectSettings::setRotateDataParameters(
    const RotateData::Parameters &rotateDataParameters) {
  _rotateDataParameters = rotateDataParameters;
}

RotateData::Parameters &ProjectSettings::getRotateDataParameters() {
  return _rotateDataParameters;
}

const RotateData::Parameters &ProjectSettings::getRotateDataParameters() const {
  return _rotateDataParameters;
}

} // namespace Data
