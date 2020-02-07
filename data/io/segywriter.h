#pragma once

#include <segyio/segy.h>

#include <QString>

namespace Data {
class SeismEvent;
namespace IO {
class SegyWriter
{
public:
    bool save(const QString& , Data::SeismEvent const * const );

private:
    segy_file *_fp{nullptr};

    int _sam_num{0};
    float _sam_intr{0};
    int _format{0};
    long _trace0{0};
    int _trace_num{0};
    int _trace_bsize{0};
  };



}
}
