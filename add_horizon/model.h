#pragma once

#include "data/seismhorizon.h"

#include <QObject>

#include <memory>


namespace AddHorizon {
class Model : public QObject {
    Q_OBJECT

public:
    explicit Model(QObject* );

    void setFilePath(const QString& );
    std::unique_ptr<Data::SeismHorizon>& getSeismHorizon();

signals:
    void notify(const QString& ) const;

private:
    void getSeismHorizonFrom(const QString& );

    QString _path;
    std::unique_ptr<Data::SeismHorizon> _horizon;
};


} // namespace AddHorizon
