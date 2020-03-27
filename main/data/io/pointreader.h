#pragma once

#include "data/seism_data_type.h"

#include <QDataStream>
#include <QFileInfo>
#include <QTextStream>

namespace Data {
namespace IO {
class PointReader {
public:
  explicit PointReader(const QFileInfo &) noexcept(false);

  bool hasNext() const;

  Point next();

  ~PointReader();

private:
  bool _isBin;

  QFile _file;

  QTextStream _instreamText;
  QDataStream _instreamBin;
};

} // namespace IO
} // namespace Data
