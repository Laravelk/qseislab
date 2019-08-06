#include "seismtrace.h"

#include <iostream> // debug

namespace Data {
SeismTrace::SeismTrace()
{}

SeismTrace::SeismTrace(const QJsonObject& json, std::unique_ptr<float[]>& data)
{
    if( !(json.contains("sampleNum") && json.contains("sampleInterval") && json.contains("cdpX") && json.contains("cdpY")) ) {
        throw std::runtime_error("Not found json-field (SeismTrace)");
    }

    _sampleNum = json["sampleNum"].toInt();
    _sampleInterval = static_cast<float>(json["sampleInterval"].toDouble());
    int size = json["size"].toInt();

    if( 0 > _sampleNum || 0 > size) {
        throw std::runtime_error("Invalid values for json-field");
    }

    _bufferSize = static_cast<unsigned>(size);

    _cdpX = json["cdpX"].toInt();
    _cdpY = json["cdpY"].toInt();

    _buffer = std::move(data);
}

int SeismTrace::getSampleNum() const
{
    return _sampleNum;
}

void SeismTrace::setSampleNum(int sampleNum)
{
    _sampleNum = sampleNum;
}

float SeismTrace::getSampleInterval() const
{
    return _sampleInterval;
}

void SeismTrace::setSampleInterval(float sampleInterval)
{
    _sampleInterval = sampleInterval;
}

int SeismTrace::getCdpX() const
{
    return _cdpX;
}

void SeismTrace::setCdpX(int cdpX)
{
    _cdpX = cdpX;
}

int SeismTrace::getCdpY() const
{
    return _cdpY;
}

void SeismTrace::setCdpY(int cdpY)
{
    _cdpY = cdpY;
}

unsigned SeismTrace::getBufferSize() const
{
    return _bufferSize;
}

const std::unique_ptr<float[]>& SeismTrace::getBuffer() const
{
    return _buffer;
}

void SeismTrace::setBuffer(unsigned size, float* buffer)
{
    assert(0 <= size);

    _bufferSize = size;

    _buffer.reset();
    _buffer = std::make_unique<float[]>(_bufferSize);
    for(unsigned i = 0; i < _bufferSize; ++i) {
        if (i < 20)
            std::cerr << buffer[i] << " ";
        _buffer[i] = buffer[i];
    }
}

QJsonObject& SeismTrace::writeToJson(QJsonObject& json) const
{
    json["sampleNum"] = _sampleNum;
    json["sampleInterval"] = static_cast<double>(_sampleInterval);
    json["cdpX"] = _cdpX;
    json["cdpY"] = _cdpY;
    json["size"] = static_cast<int>(_bufferSize);
    return json;
}


} //namespace Data
