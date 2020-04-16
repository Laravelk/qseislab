#pragma once

#include "CSVparser.hpp"

#include <QObject>

#include <memory>
#include <set>

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

  //  std::map<QUuid, std::shared_ptr<Data::SeismWell>> getSeismReceiversFrom(
  //      const QString &,
  //      const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &);

  std::list<std::shared_ptr<Data::SeismReceiver>> getSeismReceiversFrom(
      const QString &,
      const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &,
      const std::list<std::shared_ptr<Data::SeismReceiver>> &);

signals:
  void notify(const QString &) const;

private:
  static const unsigned columnNum;

  void checkHeaderCorrect(const csv::Parser &) noexcept(false);
  void settingReceiverInfo(const std::shared_ptr<Data::SeismReceiver> &,
                           const csv::Row &) noexcept(false);
  void settingChannelInfo(const std::shared_ptr<Data::SeismChannelReceiver> &,
                          const csv::Row &) noexcept(false);

  //  const std::shared_ptr<Data::SeismReceiver> &
  //  getReceiver(const int, const std::shared_ptr<Data::SeismWell> &, bool *);

  const std::shared_ptr<Data::SeismReceiver> &
  getReceiver(const int, Data::SeismWell const *const,
              std::list<std::shared_ptr<Data::SeismReceiver>> &);

  const std::shared_ptr<Data::SeismWell> &
  findWellOfName(const QString &,
                 const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &);

  //  bool receiverExistInWell(const std::shared_ptr<Data::SeismReceiver> &,
  //                           const std::shared_ptr<Data::SeismWell> &);

  //  const std::shared_ptr<Data::SeismReceiver> &
  //  getExistReceiverFromWellOfWellNum(const std::shared_ptr<Data::SeismWell>
  //  &,
  //                                    const int);

  void checkCorrect(const std::shared_ptr<Data::SeismReceiver> &,
                    const csv::Row &);
  void checkCorrect(const std::shared_ptr<Data::SeismReceiver> &,
                    const std::shared_ptr<Data::SeismReceiver> &);
  void checkCorrect(const std::shared_ptr<Data::SeismChannelReceiver> &,
                    const std::shared_ptr<Data::SeismReceiver> &);
};

} // namespace ReceiverOperation