#pragma once

#include "data/seismhorizon.h"

#include <QObject>

#include <memory>

namespace HorizonOperation {
namespace AddHorizon {
class Model : public QObject {
  Q_OBJECT

public:
  explicit Model(QObject *);

  std::unique_ptr<Data::SeismHorizon> getSeismHorizonFrom(const QString &);

signals:
  void notify(const QString &) const;

private:
  std::unique_ptr<Data::SeismHorizon> _horizon;
};

} // namespace AddHorizon
} // namespace HorizonOperation
