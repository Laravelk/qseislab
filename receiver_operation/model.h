#pragma once

#include "CSVparser.hpp"

#include <QObject>

#include <list>
#include <memory>

namespace Data {
class SeismWell;
// class SeismProject;
class SeismReceiver;
class SeismChannelReceiver;
} // namespace Data

namespace ReceiverOperation {
class Model : public QObject {
  Q_OBJECT

public:
  explicit Model(QObject *parent = nullptr);

  std::map<QUuid, std::unique_ptr<Data::SeismWell>> getSeismReceiversFrom(
      const QString &,
      const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &);

signals:
  void notify(const QString &) const;

private:
  static const unsigned columnNum;

  void checkHeaderCorrect(const csv::Parser &) noexcept(false);
  void settingRecieverInfo(const std::unique_ptr<Data::SeismReceiver> &,
                           const csv::Row &) noexcept(false);
  void settingChannelInfo(const std::unique_ptr<Data::SeismChannelReceiver> &,
                          const csv::Row &) noexcept(false);

  const std::unique_ptr<Data::SeismReceiver> &
  getReceiver(const int, std::unique_ptr<Data::SeismWell> &, bool *);

  std::unique_ptr<Data::SeismWell> &
  findWellOfName(const QString &,
                 std::map<QUuid, std::unique_ptr<Data::SeismWell>> &);

  bool receiverExistInWell(const std::unique_ptr<Data::SeismReceiver> &,
                           const std::unique_ptr<Data::SeismWell> &);

  const std::unique_ptr<Data::SeismReceiver> &
  getExistReceiverFromWellOfWellNum(const std::unique_ptr<Data::SeismWell> &,
                                    const int);

  void checkCorrect(const std::unique_ptr<Data::SeismReceiver> &,
                    const csv::Row &);
  void checkCorrect(const std::unique_ptr<Data::SeismReceiver> &,
                    const std::unique_ptr<Data::SeismReceiver> &);
  void checkCorrect(const std::unique_ptr<Data::SeismChannelReceiver> &,
                    const std::unique_ptr<Data::SeismReceiver> &);
};

} // namespace ReceiverOperation
