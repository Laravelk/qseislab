#include "testmultiplier.h"

#include "data/seismevent.h"

#include <iostream> // TODO: remove

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace Modefication {
TestMultiplier::TestMultiplier(SeismEvent *event, float multiplier)
    : _event(event), _multiplier(multiplier) {

  setText("Test Multiplier");
}

void TestMultiplier::undo() {
  std::cout << "call test-undo" << std::endl;
  for (auto &component : _event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      float *data = trace->getBuffer();
      int size = trace->getBufferSize();
      for (int i = 0; i < size; ++i) {
        data[i] = data[i] / _multiplier;
      }
    }
  }
}

void TestMultiplier::redo() {
  std::cout << "call test-redo" << std::endl;
  for (auto &component : _event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      float *data = trace->getBuffer();
      int size = trace->getBufferSize();
      for (int i = 0; i < size; ++i) {
        data[i] = data[i] * _multiplier;
      }
    }
  }
}

} // namespace Modefication
} // namespace EventOperation
