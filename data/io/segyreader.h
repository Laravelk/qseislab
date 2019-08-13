#pragma once

#include "abstractsegyreader.h"

#include <segyio/segy.h>

// TODO: где и как дефайнить такое?? может так?
#define SEGY_READER_TRACE_IN_COMPONENT 3;

namespace Data {
namespace IO {
class SegyReader : public AbstractSegyReader {
public:
  void setFilePath(const char *) noexcept(false) override;

  void readBinHeader() noexcept(false) override;

  int traceInComponent() const override;

  bool hasNextComponent() const override;

  std::unique_ptr<SeismTrace> nextTrace() noexcept(false) override;

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

  std::unique_ptr<SeismTrace> _trace;
};

} // namespace IO
} // namespace Data
