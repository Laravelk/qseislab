#pragma once

#include "data/seismhorizon.h"

#include <QFileInfo>
#include <QTextStream>


namespace Data {
namespace IO {
class SeismPointReader {
public:
    explicit SeismPointReader(const QFileInfo&) noexcept(false);

    bool hasNext() const;
    void next();

    const SeismHorizon::SeismPoint& getPoint() const;

    ~SeismPointReader();

private:
    QFile _file;
    QTextStream _instream;

    unsigned _pointNum;
    unsigned _readNum{0};

    SeismHorizon::SeismPoint _point{0,0,0,0};
};



} // namespace IO
} // namespace Data
