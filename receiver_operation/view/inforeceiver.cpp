#include "inforeceiver.h"

#include "data/seismchannelreceiver.h"
#include "data/seismreceiver.h"

#include <QBoxLayout>
#include <QPushButton>

typedef Data::SeismReceiver SeismReceiver;
typedef Data::SeismChannelReceiver SeismChannelReceiver;

namespace ReceiverOperation {
InfoReceiver::InfoReceiver(const std::unique_ptr<Data::SeismReceiver> &receiver,
                           QWidget *parent)
    : QDialog(parent), _nameLineEdit(new QLineEdit(receiver->getName())),
      _channelsTable(new QTableWidget()) {

  // Setting`s
  setWindowTitle("Receiver Info");
  setMinimumWidth(440);

  initChannelssTable(_channelsTable);
  for (auto &channel : receiver->getChannels()) {
    insertChannelInTable(channel);
  }

  QPushButton *saveButton = new QPushButton("Save");
  QPushButton *cancelButton = new QPushButton("Cancel");
  // Setting`s end

  // Connecting
  connect(saveButton, &QPushButton::clicked, [this, &receiver] {
    receiver->setName(_nameLineEdit->text());
    emit accept();
  });
  connect(cancelButton, &QPushButton::clicked, this, &InfoReceiver::reject);
  // Connecting end

  // Layout`s
  QHBoxLayout *buttonlayout = new QHBoxLayout();
  buttonlayout->addStretch(1);
  buttonlayout->addWidget(saveButton);
  buttonlayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_nameLineEdit);
  mainLayout->addWidget(_channelsTable);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonlayout);

  setLayout(mainLayout);
  // Layout`s end
}

void InfoReceiver::initChannelssTable(QTableWidget *table) {
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setColumnCount(8);

  table->setHorizontalHeaderLabels(QStringList() << "Name"
                                                 << "Channel-Num"
                                                 << "Axis-Num"
                                                 << "Orientation"
                                                 << "Motion"
                                                 << "P-Station-Correction"
                                                 << "S-Station-Correction"
                                                 << "Well-ChannelNum");
}

void InfoReceiver::insertChannelInTable(
    const std::unique_ptr<Data::SeismChannelReceiver> &channel) {
  _channelsTable->insertRow(_channelsTable->rowCount());

  int insertRow = _channelsTable->rowCount() - 1;

  _channelsTable->setItem(insertRow, 0,
                          new QTableWidgetItem(channel->getName()));

  _channelsTable->setItem(insertRow, 1,
                          new QTableWidgetItem(channel->getChannelNum()));

  _channelsTable->setItem(insertRow, 2,
                          new QTableWidgetItem(channel->getAxisNum()));

  _channelsTable->setItem(
      insertRow, 3,
      new QTableWidgetItem(
          QString("%1 %2 %3")
              .arg(static_cast<double>(std::get<0>(channel->getOrientation())),
                   0, 'f', 2)
              .arg(static_cast<double>(std::get<1>(channel->getOrientation())),
                   0, 'f', 2)
              .arg(static_cast<double>(std::get<2>(channel->getOrientation())),
                   0, 'f', 2)));

  _channelsTable->setItem(insertRow, 4,
                          new QTableWidgetItem(channel->getMotion()));

  _channelsTable->setItem(
      insertRow, 5, new QTableWidgetItem(channel->getPStationCorrection()));

  _channelsTable->setItem(
      insertRow, 6, new QTableWidgetItem(channel->getSStationCorrection()));

  _channelsTable->setItem(insertRow, 7,
                          new QTableWidgetItem(channel->getWellChannelNum()));
}

} // namespace ReceiverOperation
