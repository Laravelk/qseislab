#pragma once

#include <QObject>

#include <memory>

namespace Data {
class SeismReciever;
class SeismEvent;
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

  std::unique_ptr<Data::SeismEvent> getSeismEventFrom(const QString &);
  std::unique_ptr<Data::SeismEvent>
  getSeismEventFrom(const QString &,
                    const std::list<std::unique_ptr<Data::SeismReciever>> &);

  ~Model();

signals:
  void notify(const QString &) const;

private:
  Data::IO::AbstractSegyReader *_reader;
};

} // namespace AddEvent
} // namespace EventOperation
