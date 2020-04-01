#include "ffilteringmath.h"

#include <iostream> // TODO: delete

typedef unsigned int uint;

namespace EventMath {
void FFilteringMath::FFiltering(std::vector<float> &timevec, const int F1,
                                const int F2, const int F3, const int F4,
                                const float sampleInterval) {

  Eigen::FFT<float> fft;
  std::vector<std::complex<float>> freqvec;

  std::cerr << "H" << std::endl;
  if (0 == (timevec.size() % 2)) {
    timevec.push_back(0);
  }

  fft.fwd(freqvec, timevec);

  uint indexF1 = static_cast<uint>(
      ceil((F1 * static_cast<float>(timevec.size()) * sampleInterval) /
           static_cast<float>(MICROSECONDS_IN_SECONDS)));
  uint indexF2 = static_cast<uint>(
      ceil((F2 * static_cast<float>(timevec.size()) * sampleInterval) /
           static_cast<float>(MICROSECONDS_IN_SECONDS)));
  uint indexF3 = static_cast<uint>(
      ceil((F3 * static_cast<float>(timevec.size()) * sampleInterval) /
           static_cast<float>(MICROSECONDS_IN_SECONDS)));
  uint indexF4 = static_cast<uint>(
      ceil((F4 * static_cast<float>(timevec.size()) * sampleInterval) /
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
}
}
