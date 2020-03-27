#pragma once

#include <QObject>

#include <memory>

namespace Data {
class SeismHorizon;
}

namespace HorizonOperation {
class Model : public QObject {
  Q_OBJECT

public:
  explicit Model(QObject *);

  std::unique_ptr<Data::SeismHorizon> getSeismHorizonFrom(const QString &);

signals:
  void notify(const QString &) const;
};

} // namespace HorizonOperation
