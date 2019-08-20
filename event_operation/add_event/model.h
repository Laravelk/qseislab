#pragma once

#include "data/seismevent.h"

#include <QObject>

#include <memory>


namespace Data {
namespace IO {
class AbstractSegyReader;
}
}

namespace EventOperation {
namespace AddEvent {
class Model : public QObject {
    Q_OBJECT

public:
    explicit Model(Data::IO::AbstractSegyReader* , QObject* );

    std::unique_ptr<Data::SeismEvent> getSeismEventFrom(const QString& );

    ~Model();

signals:
    void notify(const QString& ) const;

private:
    Data::IO::AbstractSegyReader* _reader;
    std::unique_ptr<Data::SeismEvent> _event;
};


} // namespace AddEvent
} // namespace EventOperation
