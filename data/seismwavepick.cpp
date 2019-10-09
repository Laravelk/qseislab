#include "seismwavepick.h"

namespace Data {
SeismWavePick::SeismWavePick(int arrival, int polarizationBorder)
    : _arrival(arrival), _polarizationBorder(polarizationBorder) {}

int SeismWavePick::getArrival() const { return _arrival; }

void SeismWavePick::setArrival(int arrival) { _arrival = arrival; }

int SeismWavePick::getPolarizationBorder() const { return _polarizationBorder; }

void SeismWavePick::setPolarizationBorder(int polarizationBorder) {
  _polarizationBorder = polarizationBorder;
}

} // namespace Data
