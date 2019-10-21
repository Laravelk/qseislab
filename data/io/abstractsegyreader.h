#pragma once

#include <memory>

namespace Data {
class SeismComponent;
class SeismReceiver;

namespace IO {
class AbstractSegyReader {
public:
  virtual void setFilePath(const char *) = 0;

  virtual void readBinHeader() = 0;

  virtual bool hasNextComponent() const = 0;
  virtual std::unique_ptr<SeismComponent>
  nextComponent(const std::unique_ptr<SeismReceiver> &) = 0;

  virtual void close() = 0;

  virtual ~AbstractSegyReader() { ; }
};

} // namespace IO
} // namespace Data
