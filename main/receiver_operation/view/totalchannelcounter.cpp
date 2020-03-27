#include "totalchannelcounter.h"

#include "data/seismreceiver.h"

#include <QBoxLayout>

typedef Data::SeismReceiver SeismReceiver;

namespace ReceiverOperation {
TotalChannelCounter::TotalChannelCounter(QWidget *parent)
    : QWidget(parent), _count(new QLabel("0")) {

  QLabel *label = new QLabel("Total channel amount: ");
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(label);
  layout->addWidget(_count);
  layout->addStretch(1);

  setLayout(layout);
}

void TotalChannelCounter::add(SeismReceiver const *const receiver) {
  int amount = receiver->getChannelAmount();
  _numbers_map[receiver->getUuid()] = amount;
  _count->setNum(_count->text().toInt() + amount);
}

void TotalChannelCounter::update(const Data::SeismReceiver *const receiver) {
  int newAmount = receiver->getChannelAmount();
  int oldAmount = _numbers_map[receiver->getUuid()];
  _numbers_map[receiver->getUuid()] = newAmount;
  _count->setNum(_count->text().toInt() - oldAmount + newAmount);
}

bool TotalChannelCounter::remove(const QUuid &uuid) {
  if (_numbers_map.end() != _numbers_map.find(uuid)) {
    _count->setNum(_count->text().toInt() - _numbers_map[uuid]);
    _numbers_map.erase(uuid);
    return true;
  }
  return false;
}
} // namespace ReceiverOperation
