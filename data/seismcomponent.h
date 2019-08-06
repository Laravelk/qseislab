#pragma once

#include "seismtrace.h"

#include <memory>

#include <QJsonObject>


namespace Data {
class SeismComponent {
public:
    typedef enum {
        TRACE_X,
        TRACE_Y,
        TRACE_Z
    } TRACE;

    explicit SeismComponent();
    explicit SeismComponent(const QJsonObject&, std::unique_ptr<float[]>&, std::unique_ptr<float[]>&, std::unique_ptr<float[]>& ) noexcept(false);

    void setTrace(TRACE, std::unique_ptr<SeismTrace>& );
    bool addTrace(std::unique_ptr<SeismTrace>& );

    const std::unique_ptr<SeismTrace>& getTraceX() const;
    const std::unique_ptr<SeismTrace>& getTraceY() const;
    const std::unique_ptr<SeismTrace>& getTraceZ() const;

    QJsonObject& writeToJson(QJsonObject& ) const;

private:
    std::unique_ptr<SeismTrace> _traceX;
    std::unique_ptr<SeismTrace> _traceY;
    std::unique_ptr<SeismTrace> _traceZ;
};


} // namespace Data
