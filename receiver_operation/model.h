#pragma once

#include <QObject>

#include <memory>
#include <list>


namespace Data {
class SeismReceiver;
} // namespace Data

namespace ReceiverOperation {
class Model : public QObject {
    Q_OBJECT

public:
    explicit Model(QObject* parent = nullptr);

    std::list<std::unique_ptr<Data::SeismReceiver>> getSeismReceiversFrom(const QString& );

signals:
    void notify(const QString& ) const;
};

} // namespace ReceiverOperation

