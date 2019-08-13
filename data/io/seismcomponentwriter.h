#pragma once

#include "data/seismcomponent.h"

#include <QDataStream>
#include <QFileInfo>

#include <memory>
namespace Data {
class SeismComponent;
class SeismTrace;
namespace IO {
class SeismComponentWriter {
public:
  explicit SeismComponentWriter(const QFileInfo &, int, int);

  void writeComponent(const std::unique_ptr<SeismComponent> &);

  ~SeismComponentWriter();

private:
  void writeTrace(const std::unique_ptr<SeismTrace> &);

  QFile _file;
  QDataStream _outstream;
};

} // namespace IO
} // namespace Data
