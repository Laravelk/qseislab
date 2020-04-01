#include "ffilteringdatacommand.h"

#include "data/seismevent.h"

#include <iostream> // TODO: delete
#include <math.h>
#include <unsupported/Eigen/FFT>

FFilteringDataCommand::FFilteringDataCommand(
    const std::set<Data::SeismEvent *> &events,
    const FFilteringDataCommand::Parameters &parameters)
    : EventOperationUndoCommand(events), _parameters(parameters) {}

void FFilteringDataCommand::redoForOne(Data::SeismEvent *event) {
  Eigen::FFT<float> fft;
  fillOldDataList(event);
  auto oldTraces = _oldEventsTraces.at(event->getUuid());

  uint traceNumberInVector = 0;
  std::vector<std::vector<float>> newData;
  for (auto &component : event->getComponents()) {
    float sampleInterval = component->getSampleInterval();
    for (auto &trace : component->getTraces()) {
      std::vector<float> timevec = oldTraces.at(traceNumberInVector);
      traceNumberInVector++;
      EventMath::FFilteringMath::FFiltering(
          timevec, _parameters.getF1(), _parameters.getF2(),
          _parameters.getF3(), _parameters.getF4(), sampleInterval);
      newData.push_back(timevec);
    }
  }

  // new data if it's invalid

  int traceNumber = 0;
  for (auto &component : event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      float buffer[trace->getBufferSize()];
      std::vector<float> timevec = newData.at(traceNumber);
      for (uint i = 0; i < trace->getBufferSize(); i++) {
        buffer[i] = timevec[i];
      }
      trace->setBuffer(trace->getBufferSize(), buffer);
      traceNumber++;
    }
  }
  event->changeTrigger();
}

void FFilteringDataCommand::undoForOne(Data::SeismEvent *event) {
  uint traceNumberInVector = 0;
  auto oldTraces = _oldEventsTraces.at(event->getUuid());
  for (auto &component : event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      if (oldTraces.size() > traceNumberInVector) {
        auto oldBuffer = oldTraces.at(traceNumberInVector);
        auto bufferInTrace = trace->getBuffer();
        for (uint i = 0; i < oldBuffer.size(); i++) {
          bufferInTrace[i] = oldBuffer[i];
        }
        traceNumberInVector++;
      }
    }
  }
  event->changeTrigger();
}

void FFilteringDataCommand::fillOldDataList(Data::SeismEvent *event) {
  std::vector<std::vector<float>> oldTraces;
  for (auto &component : event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      std::vector<float> buffer;
      for (int i = 0; i < trace->getBufferSize(); i++) {
        buffer.push_back(trace->getBuffer()[i]);
      }
      oldTraces.push_back(buffer);
    }
  }
  _oldEventsTraces[event->getUuid()] = std::move(oldTraces);
}

int FFilteringDataCommand::Parameters::getF1() const { return _F1; }

void FFilteringDataCommand::Parameters::setF1(int f1) { _F1 = f1; }

int FFilteringDataCommand::Parameters::getF2() const { return _F2; }

void FFilteringDataCommand::Parameters::setF2(int f2) { _F2 = f2; }

int FFilteringDataCommand::Parameters::getF3() const { return _F3; }

void FFilteringDataCommand::Parameters::setF3(int f3) { _F3 = f3; }

int FFilteringDataCommand::Parameters::getF4() const { return _F4; }

void FFilteringDataCommand::Parameters::setF4(int f4) { _F4 = f4; }
