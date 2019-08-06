#include "seismcomponent.h"

namespace Data {
SeismComponent::SeismComponent()
{}

SeismComponent::SeismComponent(const QJsonObject& json, std::unique_ptr<float[]>& dataX, std::unique_ptr<float[]>& dataY, std::unique_ptr<float[]>& dataZ)
{
    if( !(json.contains("Trace X") && json.contains("Trace Y") && json.contains("Trace Z")) ) {
        throw std::runtime_error("Not found json-field (SeismComponent)");
    }

    QJsonObject obj = json["Trace X"].toObject();
    _traceX = std::make_unique<SeismTrace>(obj, dataX);

    obj = json["Trace Y"].toObject();
    _traceY = std::make_unique<SeismTrace>(obj, dataY);

    obj = json["Trace Z"].toObject();
    _traceZ = std::make_unique<SeismTrace>(obj, dataZ);
}

void SeismComponent::setTrace(TRACE idx, std::unique_ptr<SeismTrace>& trace)
{
    assert(TRACE_X <= idx && idx < TRACE_Z);

    switch (idx) {
        case TRACE_X:
            _traceY = std::move(trace);
            break;

        case TRACE_Y:
            _traceX = std::move(trace);
            break;

        case TRACE_Z:
            _traceZ = std::move(trace);
            break;
    }
}

bool SeismComponent::addTrace(std::unique_ptr<SeismTrace>& trace)
{
    if(!_traceX){
        _traceX = std::move(trace);
        return true;
    }

    if(!_traceY){
        _traceY = std::move(trace);
        return true;
    }

    if(!_traceZ){
        _traceZ = std::move(trace);
        return true;
    }

    return false;
}

const std::unique_ptr<SeismTrace>& SeismComponent::getTraceX() const
{
    return _traceX;
}

const std::unique_ptr<SeismTrace>& SeismComponent::getTraceY() const
{
    return _traceY;
}

const std::unique_ptr<SeismTrace>& SeismComponent::getTraceZ() const
{
    return _traceZ;
}

QJsonObject& SeismComponent::writeToJson(QJsonObject& json) const
{
    QJsonObject traceObj;
    json["Trace X"] = _traceX->writeToJson(traceObj);

    json["Trace Y"] = _traceY->writeToJson(traceObj);

    json["Trace Z"] = _traceZ->writeToJson(traceObj);

    return json;
}


} // namespace Data
