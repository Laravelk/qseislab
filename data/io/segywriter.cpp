#include "segywriter.h"

#include "data/seismevent.h"

namespace Data {
namespace IO {

bool SegyWriter::save(const QString& dirPath, SeismEvent const * const event) {
    _fp = segy_open((dirPath + event->getName()).toLocal8Bit().data(), "wb");
    if (!_fp) {
      throw std::runtime_error("segy_open()");
    }
}

}
}
