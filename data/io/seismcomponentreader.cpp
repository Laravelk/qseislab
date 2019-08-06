#include "seismcomponentreader.h"


namespace Data {
namespace IO {
SeismComponentReader::SeismComponentReader(const QFileInfo& fileInfo)
    :_file(fileInfo.absoluteFilePath()),
     _instream(&_file)
{
    if(!_file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("File can not be opened");
    }

    _instream >> _componentNum;
}

std::unique_ptr<float[]>& SeismComponentReader::getDataX()
{
    return  _dataX;
}

std::unique_ptr<float[]>& SeismComponentReader::getDataY()
{
    return _dataY;
}

std::unique_ptr<float[]>& SeismComponentReader::getDataZ()
{
    return _dataZ;
}


void SeismComponentReader::next()
{
    // TODO: проверять можно считывать из потока столько данных сколько нужно
    if(_componentNum <= _readNum) {
        throw std::runtime_error("Data in the file ended");
    }

    unsigned num = 0;
    _instream >> num;
    _dataX = std::make_unique<float[]>(num);
    for(unsigned i = 0; i < num; ++i) {
        _instream >> _dataX[i];
    }

    _instream >> num;
    _dataY = std::make_unique<float[]>(num);
    for(unsigned i = 0; i < num; ++i) {
        _instream >> _dataY[i];
    }

    _instream >> num;
    _dataZ = std::make_unique<float[]>(num);
    for(unsigned i = 0; i < num; ++i) {
        _instream >> _dataZ[i];
    }

    ++_readNum;
}

SeismComponentReader::~SeismComponentReader()
{
    _file.close();
}


} // namespace IO
} // namespace Data
