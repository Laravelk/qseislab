#pragma once

#include <QFileInfo>
#include <QTextStream>


namespace Data {
class SeismPoint;
namespace IO {
class SeismPointWriter {
public:
    explicit SeismPointWriter(const QFileInfo& , int);

    void writePoint(const SeismPoint& );

    ~SeismPointWriter();

private:
    QFile _file;
    QTextStream _outstream;
};


} // namespace IO
} // namespace Data
