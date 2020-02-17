#pragma once

#include "data/seismhorizon.h"

#include <QDataStream>
#include <QFileInfo>

namespace Data {
namespace IO {
class PointWriter {
public:
  explicit PointWriter(const QFileInfo &);

  void writePoint(const Point &);

  ~PointWriter();

private:
  QFile _file;
  QDataStream _outstream;
};

} // namespace IO
} // namespace Data
