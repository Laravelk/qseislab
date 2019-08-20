#pragma once

#include "data/seismhorizon.h"

#include <QDataStream>
#include <QFileInfo>

namespace Data {
namespace IO {
class PointReader {
public:
  explicit PointReader(const QFileInfo &) noexcept(false);

  bool hasNext() const;

  Point next();

  ~PointReader();

private:
  QFile _file;
  QDataStream _instream;
};

} // namespace IO
} // namespace Data
