#pragma once

#include <memory>


namespace Data {
class SeismComponent;

namespace IO {
class AbstractSegyReader {
public:
    virtual void setFilePath(const char* ) = 0;

    virtual void readBinHeader() = 0;

    virtual  bool hasNextComponent() const = 0;
    virtual std::unique_ptr<SeismComponent> nextComponent() = 0;

    virtual void close() = 0;

    virtual ~AbstractSegyReader(){;}
};


} // namespace Reader
} // namespace Data
