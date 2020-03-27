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
      std::vector<std::complex<float>> freqvec;
      traceNumberInVector++;

      if (0 == (timevec.size() % 2)) {
        timevec.push_back(0);
      }

      fft.fwd(freqvec, timevec);

      uint indexF1 = static_cast<uint>(
          ceil((_parameters.getF1() * static_cast<float>(timevec.size()) *
                sampleInterval) /
               static_cast<float>(MICROSECONDS_IN_SECONDS)));
      uint indexF2 = static_cast<uint>(
          ceil((_parameters.getF2() * static_cast<float>(timevec.size()) *
                sampleInterval) /
               static_cast<float>(MICROSECONDS_IN_SECONDS)));
      uint indexF3 = static_cast<uint>(
          ceil((_parameters.getF3() * static_cast<float>(timevec.size()) *
                sampleInterval) /
               static_cast<float>(MICROSECONDS_IN_SECONDS)));
      uint indexF4 = static_cast<uint>(
          ceil((_parameters.getF4() * static_cast<float>(timevec.size()) *
                sampleInterval) /
               static_cast<float>(MICROSECONDS_IN_SECONDS)));

      if (indexF1 < 0 || !(indexF1 < indexF2) || !(indexF2 < indexF3) ||
          !(indexF3 < indexF4) || indexF4 > timevec.size() / 2) {
        return;
      }

      uint indexF5 = timevec.size() - indexF4;
      uint indexF6 = timevec.size() - indexF3;
      uint indexF7 = timevec.size() - indexF2;
      uint indexF8 = timevec.size() - indexF1;

      // scalar f1 - f4 zone
      for (uint i = 1; i <= indexF1; i++) {
        freqvec[i] = 0;
      }

      float h = indexF2 - indexF1;
      for (uint i = indexF1 + 1; i <= indexF2; i++) {
        float scalar = (1.0f * i - indexF1) / h;
        freqvec[i] *= scalar;
      }

      float d = indexF4 - indexF3;
      for (uint i = indexF3 + 1; i <= indexF4; i++) {
        float scalar = (1.0f * indexF4 - i) / d;
        freqvec[i] *= scalar;
      }

      for (uint i = indexF4 + 1; i <= indexF5; i++) {
        freqvec[i] = 0;
      }

      float d2 = indexF6 - indexF5;
      for (uint i = indexF5; i < indexF6; i++) {
        float scalar = (1.0f * i - indexF5) / d2;
        freqvec[i] *= scalar;
      }

      float h2 = indexF8 - indexF7;
      for (uint i = indexF7; i < indexF8; i++) {
        float scalar = (1.0f * indexF8 - i) / h2;
        freqvec[i] *= scalar;
      }

      for (uint i = indexF8; i < timevec.size(); i++) {
        freqvec[i] = 0;
      }

      fft.inv(timevec, freqvec);

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
