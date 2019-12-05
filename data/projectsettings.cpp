#include "projectsettings.h"


namespace Data {
ProjectSettings::ProjectSettings()
{

}

const TestIndividualCommand::Parameters &ProjectSettings::getTestMultParameters() const
{
    return _testMultParameters;
}

void ProjectSettings::setTestMultParameters(const TestIndividualCommand::Parameters &testMultParameters)
{
    _testMultParameters = testMultParameters;
}

} // namespace Data {
