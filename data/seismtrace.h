#pragma once

#include <memory>

#include <QJsonObject>


namespace Data {
class SeismTrace {
public:
    explicit SeismTrace();
    explicit SeismTrace(const QJsonObject&, std::unique_ptr<float[]>& ) noexcept(false);

    int getSampleNum() const;
    void setSampleNum(int);

    float getSampleInterval() const;
    void setSampleInterval(float);

    int getCdpX() const;
    void setCdpX(int);

    int getCdpY() const;
    void setCdpY(int);

    unsigned getBufferSize() const;

    const std::unique_ptr<float[]>& getBuffer() const;
    void setBuffer(unsigned, float* );

    QJsonObject& writeToJson(QJsonObject& ) const;

private:
    int _sampleNum{0};
    float _sampleInterval{0.0};
    int _cdpX{0};
    int _cdpY{0};

//    int _dataFormat; // use later

    unsigned _bufferSize{0};
    std::unique_ptr<float[]> _buffer;
};


} // namespace Data
