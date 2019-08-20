#pragma once

#include "defines.h"

#include <QDataStream>
#include <QFileInfo>

#include <memory>


namespace Data {
class SesismComponent;
namespace IO {
class SeismComponentReader {
public:
    explicit SeismComponentReader(const QFileInfo& ) noexcept(false);

    bool hasNext() const;

    std::vector<std::pair<uint32_t, std::unique_ptr<float[]>>>& nextData();

    ~SeismComponentReader();

private:
    QFile _file;
    QDataStream _instream;

    std::vector<std::pair<uint32_t, std::unique_ptr<float[]>>> _data;
};


} // namespace IO
} // namespace Data
