#pragma once

#include "CSVparser.hpp"

#include <QObject>

#include <list>
#include <memory>

namespace Data {
class SeismWell;
class SeismReceiver;
class SeismChannelReceiver;
} // namespace Data

namespace ReceiverOperation {
class Model : public QObject {
  Q_OBJECT

public:
  explicit Model(QObject *parent = nullptr);

  std::map<QUuid, std::shared_ptr<Data::SeismWell>> getSeismReceiversFrom(
      const QString &,
      const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &);

signals:
  void notify(const QString &) const;

private:
  static const unsigned columnNum;

  void checkHeaderCorrect(const csv::Parser &) noexcept(false);
  void settingRecieverInfo(const std::shared_ptr<Data::SeismReceiver> &,
                           const csv::Row &) noexcept(false);
  void settingChannelInfo(const std::shared_ptr<Data::SeismChannelReceiver> &,
                          const csv::Row &) noexcept(false);

  const std::shared_ptr<Data::SeismReceiver> &
  getReceiver(const int, const std::shared_ptr<Data::SeismWell> &, bool *);

  const std::shared_ptr<Data::SeismWell> &
  findWellOfName(const QString &,
                 std::map<QUuid, std::shared_ptr<Data::SeismWell>> &);

  bool receiverExistInWell(const std::shared_ptr<Data::SeismReceiver> &,
                           const std::shared_ptr<Data::SeismWell> &);

  const std::shared_ptr<Data::SeismReceiver> &
  getExistReceiverFromWellOfWellNum(const std::shared_ptr<Data::SeismWell> &,
                                    const int);

  void checkCorrect(const std::shared_ptr<Data::SeismReceiver> &,
                    const csv::Row &);
  void checkCorrect(const std::shared_ptr<Data::SeismReceiver> &,
                    const std::shared_ptr<Data::SeismReceiver> &);
  void checkCorrect(const std::shared_ptr<Data::SeismChannelReceiver> &,
                    const std::shared_ptr<Data::SeismReceiver> &);
};

} // namespace ReceiverOperation
