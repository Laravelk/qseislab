#pragma once

#include "data/seismhorizon.h"

#include <QDataStream>
#include <QFileInfo>

namespace Data {
namespace IO {
class SeismPointReader {
public:
  explicit SeismPointReader(const QFileInfo &) noexcept(false);

  bool hasNext() const;

  SeismHorizon::SeismPoint next();

  ~SeismPointReader();

private:
  QFile _file;
  QDataStream _instream;
};

} // namespace IO
} // namespace Data
