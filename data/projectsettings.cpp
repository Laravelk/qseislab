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

// void ProjectSettings::setNewTestMultParameters(
//    const NewTestIndividualCommand::Parameters &newTestParameters) {
//  _newTestMultParameters = newTestParameters;
//}

// NewTestIndividualCommand::Parameters &
// ProjectSettings::getNewTestMultParameters() {
//  return _newTestMultParameters;
//}

// const NewTestIndividualCommand::Parameters &
// ProjectSettings::getNewTestMultParameters() const {
//  return _newTestMultParameters;
//}

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

MovePick::Parameters &ProjectSettings::getMovePickParameters() {
  return _movePickParameters;
}

const MovePick::Parameters &ProjectSettings::getMovePickParameters() const {
  return _movePickParameters;
}

void ProjectSettings::setRemovePickParameters(
    const RemovePick::Parameters &removePickParameters) {
  _removePickParameters = removePickParameters;
}

RemovePick::Parameters &ProjectSettings::getRemovePickParameters() {
  return _removePickParameters;
}

const RemovePick::Parameters &ProjectSettings::getRemovePickParameters() const {
  return _removePickParameters;
}

void ProjectSettings::setAddPickParameters(
    const AddPick::Parameters &addPickParameters) {
  _addPickParameters = addPickParameters;
}

AddPick::Parameters &ProjectSettings::getAddPickParameters() {
  return _addPickParameters;
}

const AddPick::Parameters &ProjectSettings::getAddPickParameters() const {
  return _addPickParameters;
}

void ProjectSettings::setMovePickParameters(
    const MovePick::Parameters &movePickParameters) {
  _movePickParameters = movePickParameters;
}

void ProjectSettings::setFFilteringParameters(
    const FFilteringDataCommand::Parameters &ffilteringParameters) {
  _ffilteringParameters = ffilteringParameters;
}

FFilteringDataCommand::Parameters &ProjectSettings::getFFilteringParameters() {
  return _ffilteringParameters;
}

const FFilteringDataCommand::Parameters &
ProjectSettings::getFFilteringParameters() const {
    return _ffilteringParameters;
}

void ProjectSettings::setPolarizationAnalysisParameters(const PolarizationAnalysisCompute::Parameters &polarizationParameters)
{
    _polarizationParameters = polarizationParameters;
}

PolarizationAnalysisCompute::Parameters &ProjectSettings::getPolarizationParameters()
{
    return _polarizationParameters;
}

const PolarizationAnalysisCompute::Parameters &ProjectSettings::getPolarizationParameters() const
{
    return _polarizationParameters;
}
} // namespace Data
