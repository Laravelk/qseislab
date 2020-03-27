#pragma once

#include <QObject>

#include <memory>

namespace Data {
class SeismWell;
}

namespace WellOperation {
class Model : public QObject {
  Q_OBJECT

public:
  explicit Model(QObject *);

  std::shared_ptr<Data::SeismWell> getSeismWellFrom(const QString &);

signals:
  void notify(const QString &) const;
};

} // namespace WellOperation
