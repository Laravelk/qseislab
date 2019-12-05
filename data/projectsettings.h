#pragma once

#include "event_operation/modification/commands/testindividualcommand.h"

namespace Data {
class ProjectSettings {
public:
    explicit ProjectSettings();

    const TestIndividualCommand::Parameters& getTestMultParameters() const;
    void setTestMultParameters(const TestIndividualCommand::Parameters& );

private:
    TestIndividualCommand::Parameters _testMultParameters;
};
} // namespace Data

