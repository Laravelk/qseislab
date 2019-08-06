#pragma once

#include "abstractsegyreader.h"

#include <segyio/segy.h>
#include <string>


namespace Data {
namespace IO {
class SegyReader : public AbstractSegyReader{
public:
    explicit SegyReader(const char* );

    bool isValid() const override;

    bool hasNextComponent() const override;
    SeismComponent* nextComponent() override;

    const char* getErrMsg() const override;

    ~SegyReader() override;

private:
    int readBinHeader() override;
    int errmsg(int, const char*) override;

    bool _isValid{false};
    int _alreadyRead{0};

    segy_file* _fp{nullptr};

    int _sam_num{0};
    float _sam_intr{0};
    int _format{0};
    long _trace0{0};
    int _trace_num{0};
    int _trace_bsize{0};

    std::string _errmsg;
};


} // namespace Reader
} // namespace Data
