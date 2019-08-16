#pragma once

#include "data/seismhorizon.h"

#include <QDataStream>
#include <QFileInfo>

namespace Data {
namespace IO {
class SeismPointWriter {
public:
  explicit SeismPointWriter(const QFileInfo &);

  void writePoint(const SeismHorizon::SeismPoint &);

  ~SeismPointWriter();

private:
  QFile _file;
  QDataStream _outstream;
};

} // namespace IO
} // namespace Data
