#pragma once

#include <QFileInfo>
#include <QTextStream>

#include <memory>


namespace Data {
class SeismComponent;
class SeismTrace;
namespace IO {
class SeismComponentWriter {
public:
    explicit SeismComponentWriter(const QFileInfo& , int);

    void writeComponent(const std::unique_ptr<SeismComponent>& );

    ~SeismComponentWriter();

private:
    void writeTrace(const std::unique_ptr<SeismTrace>& );

    QFile _file;
    QTextStream _outstream;
};


} // namespace IO
} // namespace Data
