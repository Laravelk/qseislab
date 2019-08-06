#pragma once

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <memory>


namespace Data {
namespace IO {
class SeismComponentReader {
public:
    explicit SeismComponentReader(const QFileInfo& ) noexcept(false);

    std::unique_ptr<float[]>& getDataX();
    std::unique_ptr<float[]>& getDataY();
    std::unique_ptr<float[]>& getDataZ();

    void next();

    ~SeismComponentReader();

private:
    QFile _file;
    QTextStream _instream;

    unsigned _componentNum;
    unsigned _readNum{0};

    std::unique_ptr<float[]> _dataX;
    std::unique_ptr<float[]> _dataY;
    std::unique_ptr<float[]> _dataZ;
};


} // namespace IO
} // namespace Data
