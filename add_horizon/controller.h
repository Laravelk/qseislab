#pragma once

#include "model.h"

#include <QObject>

#include <memory>


namespace AddHorizon {
class Controller : public QObject {
    Q_OBJECT

public:
    explicit Controller(QObject* parent = nullptr);

signals:
    void sendHorizon(std::unique_ptr<Data::SeismHorizon>& ) const;
    void finished() const;

private slots:
    void recvFilePath(const QString& );
    void recvNotification(const QString& );
    void finish(int);

private:
    Model* _model;

    std::unique_ptr<Data::SeismHorizon> _horizon;
};


} // namespace AddHorizon
