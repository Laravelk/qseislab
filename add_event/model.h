#pragma once

#include "data/seismevent.h"

#include <QObject>

#include <memory>


namespace AddEvent {
class Model : public QObject {
    Q_OBJECT

public:
    explicit Model(QObject* );

    void setFilePath(const QString& );
    std::unique_ptr<Data::SeismEvent>& getSeismEvent();

signals:
    void changed() const;
    void notify(const QString& ) const;

private:
    void getSeismEventFrom(const QString& );

    QString _path;
    std::unique_ptr<Data::SeismEvent> _event;
};

} // namespace AddEvent
