#pragma once

#include <memory>

#include <QJsonObject>


namespace Data {
class SeismTrace {
public:
    explicit SeismTrace();
    explicit SeismTrace(const QJsonObject&, std::unique_ptr<float[]> ) noexcept(false);

    float getSampleInterval() const;
    void setSampleInterval(float);

    float getPWaveArrival() const;
    void setPWaveArrival(float);

    float getSWaveArrival() const;
    void setSWaveArrival(float);

    float getMaxValue() const;

    unsigned getBufferSize() const;

    const std::unique_ptr<float[]>& getBuffer() const;
    void setBuffer(unsigned, float* );

    QJsonObject& writeToJson(QJsonObject& ) const;

private:
    float _sampleInterval{0.0};

    float _pWaveArrival{0.0};
    float _sWaveArrival{0.0};

    float _maxValue{-1.0};

//    int _dataFormat; // NOTE: use later

    unsigned _bufferSize{0};
    std::unique_ptr<float[]> _buffer;
};


} // namespace Data
