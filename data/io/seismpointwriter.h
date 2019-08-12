#pragma once

#include "data/seismhorizon.h"

#include <QFileInfo>
#include <QTextStream>


namespace Data {
namespace IO {
class SeismPointWriter {
public:
    explicit SeismPointWriter(const QFileInfo& , int);

    void writePoint(const SeismHorizon::SeismPoint& );

    ~SeismPointWriter();

private:
    QFile _file;
    QTextStream _outstream;
};


} // namespace IO
} // namespace Data
