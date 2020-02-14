#pragma once

#include <segyio/segy.h>

#include <QFileInfo>

namespace Data {
class SeismEvent;
namespace IO {
class SegyWriter
{
public:
    void save(const QFileInfo& , Data::SeismEvent * const ) noexcept(false);

private:
    void write_binheader(Data::SeismEvent const * const);
    void write_textheader(Data::SeismEvent const * const);

    segy_file *_fp{nullptr};

    int _sam_num{0};
    float _sam_intr{0};
    int _format{0};
    long _trace0{0};
    int _trace_bsize{0};
  };



}
}
