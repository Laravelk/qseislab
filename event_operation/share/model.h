#pragma once

#include <QObject>

#include <memory>

namespace Data {
class SeismComponent;
// class SeismWell;
class SeismReceiver;
namespace IO {
class AbstractSegyReader;
}
} // namespace Data

namespace EventOperation {
class Model : public QObject {
  Q_OBJECT

public:
  explicit Model(Data::IO::AbstractSegyReader *, QObject *);

  //  std::list<std::shared_ptr<Data::SeismComponent>>
  //  getSeismComponents(const std::shared_ptr<Data::SeismWell> &, const QString
  //  &);

  std::list<std::shared_ptr<Data::SeismComponent>>
  getSeismComponents(const std::list<std::shared_ptr<Data::SeismReceiver>> &,
                     const QString &);

  ~Model();

signals:
  void notify(const QString &) const;

private:
  Data::IO::AbstractSegyReader *_reader;
};

} // namespace EventOperation
