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

  void writeComponent(const std::shared_ptr<SeismComponent> &);

  ~SeismComponentWriter();

private:
  void writeTrace(const std::shared_ptr<SeismTrace> &);

  QFile _file;
  QDataStream _outstream;
};

} // namespace IO
} // namespace Data
