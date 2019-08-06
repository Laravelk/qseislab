#pragma once


namespace Data {
class SeismComponent;

namespace IO {
class AbstractSegyReader {
public:
    virtual bool isValid() const = 0;

    virtual  bool hasNextComponent() const = 0;
    virtual SeismComponent* nextComponent() = 0;

    virtual const char* getErrMsg() const = 0;

    virtual ~AbstractSegyReader(){;}

protected:
    virtual int readBinHeader() = 0;
    virtual int errmsg(int, const char*) = 0;
};


} // namespace Reader
} // namespace Data
