#pragma once

//#include "defines.h"
#include "abstractsegyreader.h"

#include <segyio/segy.h>

namespace Data {
namespace IO {
class SegyReader : public AbstractSegyReader {
public:
  void setFilePath(const char *) noexcept(false) override;

  void readBinHeader() noexcept(false) override;

  bool hasNextComponent() const override;

  std::unique_ptr<SeismComponent> nextComponent(int) noexcept(false) override;

  void close() override;

private:
  int _alreadyRead{0};

  segy_file *_fp{nullptr};

  int _sam_num{0};
  float _sam_intr{0};
  int _format{0};
  long _trace0{0};
  int _trace_num{0};
  int _trace_bsize{0};
};

} // namespace IO
} // namespace Data
