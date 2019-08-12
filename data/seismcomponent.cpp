#include "seismcomponent.h"

#include <QJsonArray>


namespace Data {
SeismComponent::SeismComponent()
{}

SeismComponent::SeismComponent(const QJsonObject& json, std::vector<std::unique_ptr<float[]>>& data)
{
    if( !(json.contains("Traces") && json.contains("maxValue")) ) {
        throw std::runtime_error("Not found json-field (SeismComponent)");
    }

    _maxValue = static_cast<float>(json["maxValue"].toDouble());


    QJsonArray tracesArray( json["Traces"].toArray() );

    if( static_cast<int>(data.size()) != tracesArray.count()) {
        throw std::runtime_error("Traces-size in bin-file does not match traces-size in json-file (SeismComponent)");
    }

    unsigned i = 0;
    for(auto objTrace : tracesArray) {
        auto seismTrace = std::make_unique<SeismTrace>(objTrace.toObject(), std::move(data[i++]));
        _traces.push_back(std::move(seismTrace));
    }
}

float SeismComponent::getMaxValue() const
{
    return _maxValue;
}

void SeismComponent::addTrace(std::unique_ptr<SeismTrace> trace)
{
    if(_maxValue < trace->getMaxValue()) {
        _maxValue = trace->getMaxValue();
    }

    _traces.push_back(std::move(trace));
}

unsigned SeismComponent::getTracesNumber() const
{
    return static_cast<unsigned>(_traces.size());
}

const std::vector<std::unique_ptr<SeismTrace>>& SeismComponent::getTraces() const
{
    return _traces;
}

QJsonObject& SeismComponent::writeToJson(QJsonObject& json) const
{
    json["maxValue"] = static_cast<double>(_maxValue);

    QJsonArray tracesArray;
    QJsonObject traceObj;
    for(const std::unique_ptr<SeismTrace>& trace : _traces) {
        tracesArray.append( trace->writeToJson(traceObj) );
    }

    json["Traces"] = tracesArray;

    return json;
}


} // namespace Data
