#pragma once

#include <QLabel>
#include <QWidget>

#include <map>
#include <memory>

namespace Data {
class SeismReceiver;
class SeismChannelReceiver;
} // namespace Data

namespace ReceiverOperation {
class TotalChannelCounter : public QWidget {
  Q_OBJECT

public:
  explicit TotalChannelCounter(QWidget *parent = nullptr);

  void add(const std::unique_ptr<Data::SeismReceiver> &);
  bool remove(const QUuid &);

private:
  QLabel *_count;

  std::map<QUuid, int> _numbers_map;
};
} // namespace ReceiverOperation
