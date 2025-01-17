#pragma once

#include "segy_files_params.h"

#include <QDataStream>
#include <QFileInfo>

#include <memory>

namespace Data {
class SeismTrace;
namespace IO {
class SeismComponentReader {
public:
  explicit SeismComponentReader(const QFileInfo &) noexcept(false);

  bool hasNext() const;

  //  std::vector<std::pair<uint32_t, std::unique_ptr<float[]>>> &nextData();
  std::unique_ptr<SeismTrace> nextTrace();

  ~SeismComponentReader();

private:
  QFile _file;
  QDataStream _instream;

  //  std::vector<std::pair<uint32_t, std::unique_ptr<float[]>>> _data;
};

} // namespace IO
} // namespace Data
