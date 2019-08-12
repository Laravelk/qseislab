#pragma once

#include <QDataStream>
#include <QFile>
#include <QFileInfo>

#include <memory>


namespace Data {
namespace IO {
class SeismDataComponentReader {
public:
    explicit SeismDataComponentReader(const QFileInfo& ) noexcept(false);

    std::vector<std::unique_ptr<float[]>>& getData();

    void next();

    ~SeismDataComponentReader();

private:
    QFile _file;
    QDataStream _instream;

    int _componentNum;
    int _tracesInComponent;
    int _readNum{0};

    std::vector<std::unique_ptr<float[]>> _data;
};


} // namespace IO
} // namespace Data
