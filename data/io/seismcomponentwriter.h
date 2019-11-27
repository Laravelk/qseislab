#pragma once

#include "data/seismcomponent.h"
#include "segy_files_params.h"

#include <QDataStream>
#include <QFileInfo>

#include <memory>

namespace Data {
namespace IO {
class SeismComponentWriter {
public:
  explicit SeismComponentWriter(const QFileInfo &);

  void writeComponent(SeismComponent const *const);

  ~SeismComponentWriter();

private:
  void writeTrace(SeismTrace const *const);

  QFile _file;
  QDataStream _outstream;
};

} // namespace IO
} // namespace Data
