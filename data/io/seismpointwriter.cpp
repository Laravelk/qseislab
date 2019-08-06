#include "seismpointwriter.h"

#include "data/seismpoint.h"


namespace Data {
namespace IO {
SeismPointWriter::SeismPointWriter(const QFileInfo& fileInfo, int pointNum)
    :_file(fileInfo.absoluteFilePath()),
     _outstream(&_file)
{
    if(!_file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("File can not be opened");
    }

    _outstream << pointNum;
}

void SeismPointWriter::writePoint(const SeismPoint& point)
{
    _outstream << point.getX() << point.getY() << point.getZ() << point.getVal();
}

SeismPointWriter::~SeismPointWriter()
{
    _file.close();
}


} // namespace IO
} // namespace Data
