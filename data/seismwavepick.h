#pragma once

namespace Data {
class SeismWavePick {
public:
  enum Type { LEFT_BORDER, RIGHT_BORDER, PWAVE, SWAVE };

  SeismWavePick(int arrival = 0, int polarizationBorder = 40000);

  int getArrival() const;
  void setArrival(int);

  int getPolarizationBorder() const;
  void setPolarizationBorder(int);

private:
  int _arrival;
  int _polarizationBorder;
};

} // namespace Data
