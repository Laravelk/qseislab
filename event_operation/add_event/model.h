#pragma once

#include <QObject>

#include <memory>

namespace Data {
class SeismComponent;
class SeismWell;
namespace IO {
class AbstractSegyReader;
}
} // namespace Data

namespace EventOperation {
namespace AddEvent {
class Model : public QObject {
  Q_OBJECT

public:
  explicit Model(Data::IO::AbstractSegyReader *, QObject *);

  std::list<std::unique_ptr<Data::SeismComponent>>
  getSeismComponents(const std::unique_ptr<Data::SeismWell> &, const QString &);

  ~Model();

signals:
  void notify(const QString &) const;

private:
  Data::IO::AbstractSegyReader *_reader;
};

} // namespace AddEvent
} // namespace EventOperation
