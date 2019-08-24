#pragma once

#include <memory>

namespace Data {
class SeismComponent;

namespace IO {
class AbstractSegyReader {
public:
  virtual void setFilePath(const char *) = 0;

  virtual void readBinHeader() = 0;

  virtual bool hasNextComponent() const = 0;
  virtual std::unique_ptr<SeismComponent> nextComponent(int) = 0;

  virtual void close() = 0;

  virtual ~AbstractSegyReader() { ; }
};

} // namespace IO
} // namespace Data
