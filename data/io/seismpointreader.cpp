#include "seismpointreader.h"


namespace Data {
namespace IO {
SeismPointReader::SeismPointReader(const QFileInfo& fileInfo)
    :_file(fileInfo.absoluteFilePath()),
     _instream(&_file)
{
    if(!_file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("File can not be opened");
    }

    _instream >> _pointNum;

    if(0 < _pointNum) {
        next();
    }
}

bool SeismPointReader::hasNext() const
{
    return _pointNum >= _readNum;
}

void SeismPointReader::next()
{
    if(_pointNum < _readNum) {
        throw std::runtime_error("Data in the file ended");
    }

    float x;
    float y;
    float z;
    float val;

    _instream >> x >> y >> z >> val;

    _point = SeismPoint(x, y, z, val);

    ++_readNum;
}

const SeismPoint& SeismPointReader::getPoint() const
{
    return _point;
}

SeismPointReader::~SeismPointReader()
{
    _file.close();
}


} // namespace IO
} // namespace Data
