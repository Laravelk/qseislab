#include "tableassistant.h"

#include "data/seismhorizon.h"
#include "data/seismreceiver.h"
#include "data/seismwell.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QPushButton>

#include <QTableWidget>

#include <assert.h>

typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismWell SeismWell;
typedef Data::SeismReceiver SeismReceiver;
typedef Data::SeismChannelReceiver SeismChannelReceiver;

TableAssistant::TableAssistant(Mode mode, QWidget *parent)
    : QFrame(parent), _mode(mode), _table(new QTableWidget()) {
  // Settings
  _table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  _table->setSortingEnabled(true);
  _table->setSelectionBehavior(QAbstractItemView::SelectRows);
  _table->setSelectionMode(QAbstractItemView::SingleSelection);
  _table->verticalHeader()->setVisible(false);

  switch (_mode) {
  case ForHorizons:
    forHorizons();
    break;
  case ForWells:
    forWells();
    break;
  case ForReceivers:
    forReceivers();
    break;
  case ForReceiverChannels:
    forReceiverChannels();
    break;
  default:
    assert(false && "Unsupported TableAssistant`s mode");
  }

  // Conections
  connect(_table, &QTableWidget::cellDoubleClicked, [this](int row, int) {
    emit viewClicked(_table->item(row, 0)->data(Qt::DisplayRole).toUuid());
  });
  // Conections end

  // Layouts
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_table);
  //  mainLayout->addStretch(1);
  setLayout(mainLayout);
  // Layouts end
}

bool TableAssistant::remove(const QUuid &uuid) {
  for (int row = 0; row < _table->rowCount(); ++row) {
    if (uuid == _table->item(row, 0)->data(Qt::DisplayRole).toUuid()) {
      _table->removeRow(row);
      return true;
    }
  }
  return false;
}

void TableAssistant::clearTable() {
  const int end = _table->rowCount();
  for (int i = 0; i < end; ++i) {
    _table->removeRow(0);
  }
}

// Horizons
void TableAssistant::forHorizons() {
  assert(ForHorizons == _mode);

  _table->setColumnCount(7);
  _table->setColumnHidden(0, true);

  _table->setHorizontalHeaderLabels(QStringList() << "uuid"
                                                  << "Name"
                                                  << "Point Amount"
                                                  << "Nx"
                                                  << "Ny"
                                                  << ""
                                                  << "");

  _table->resizeColumnsToContents();

  auto horizontalHeaderObjectTable = _table->horizontalHeader();
  horizontalHeaderObjectTable->setSectionResizeMode(
      6, QHeaderView::Fixed); // fixed remove-button section
  horizontalHeaderObjectTable->setSectionResizeMode(
      5, QHeaderView::Stretch); // stretching pred-last section
  horizontalHeaderObjectTable->setDefaultAlignment(Qt::AlignLeft);
}

template <>
void TableAssistant::add<SeismHorizon>(SeismHorizon const *const horizon) {
  assert(ForHorizons == _mode);

  _table->setSortingEnabled(false);

  _table->insertRow(_table->rowCount());
  int row = _table->rowCount() - 1;

  auto &uuid = horizon->getUuid();

  _table->setItem(row, 0, new QTableWidgetItem(uuid.toString()));

  _table->setItem(row, 1, new QTableWidgetItem(horizon->getName()));

  _table->setItem(
      row, 2,
      new QTableWidgetItem(QString::number(horizon->getPointsAmount())));

  _table->setItem(row, 3,
                  new QTableWidgetItem(QString::number(horizon->getNx())));

  _table->setItem(row, 4,
                  new QTableWidgetItem(QString::number(horizon->getNy())));

  QPushButton *removeButton = new QPushButton();
  QIcon icon(":/icons/remove.png");
  removeButton->setStyleSheet(
      "background-color:transparent; border-style: outset");
  removeButton->setIcon(icon);
  _table->setCellWidget(row, 6, removeButton);
  connect(removeButton, &QPushButton::clicked,
          [this, &uuid]() { emit removeClicked(uuid); });

  _table->resizeColumnsToContents();
  _table->setSortingEnabled(true);
}

template <>
void TableAssistant::update<SeismHorizon>(SeismHorizon const *const horizon) {
  assert(ForHorizons == _mode);

  _table->setSortingEnabled(false);

  auto &uuid = horizon->getUuid();
  for (int row = 0; row < _table->rowCount(); ++row) {
    if (uuid == _table->item(row, 0)->data(Qt::DisplayRole).toUuid()) {
      _table->item(row, 1)->setData(Qt::DisplayRole, horizon->getName());
      _table->item(row, 2)->setData(Qt::DisplayRole,
                                    horizon->getPointsAmount());
      _table->item(row, 3)->setData(Qt::DisplayRole, horizon->getNx());
      _table->item(row, 4)->setData(Qt::DisplayRole, horizon->getNy());
      break;
    }
  }

  _table->resizeColumnsToContents();
  _table->setSortingEnabled(true);
}
// Horizons end

// Wells
void TableAssistant::forWells() {
  assert(ForWells == _mode);

  _table->setColumnCount(5);
  _table->setColumnHidden(0, true);

  _table->setHorizontalHeaderLabels(QStringList() << "uuid"
                                                  << "Name"
                                                  << "Point Amount"
                                                  << ""
                                                  << "");

  _table->resizeColumnsToContents();

  auto horizontalHeaderObjectTable = _table->horizontalHeader();
  horizontalHeaderObjectTable->setSectionResizeMode(
      4, QHeaderView::Fixed); // fixed remove-button section
  horizontalHeaderObjectTable->setSectionResizeMode(
      3, QHeaderView::Stretch); // stretching pred-last section
  horizontalHeaderObjectTable->setDefaultAlignment(Qt::AlignLeft);
}

template <> void TableAssistant::add<SeismWell>(SeismWell const *const well) {
  assert(ForWells == _mode);

  _table->setSortingEnabled(false);

  _table->insertRow(_table->rowCount());
  int row = _table->rowCount() - 1;

  auto &uuid = well->getUuid();
  _table->setItem(row, 0, new QTableWidgetItem(uuid.toString()));

  _table->setItem(row, 1, new QTableWidgetItem(well->getName()));

  _table->setItem(
      row, 2, new QTableWidgetItem(QString::number(well->getPointsAmount())));

  QPushButton *removeButton = new QPushButton();
  QIcon icon(":/icons/remove.png");
  removeButton->setStyleSheet(
      "background-color:transparent; border-style: outset");
  removeButton->setIcon(icon);
  _table->setCellWidget(row, 4, removeButton);
  connect(removeButton, &QPushButton::clicked,
          [this, &uuid]() { emit removeClicked(uuid); });

  _table->resizeColumnsToContents();
  _table->setSortingEnabled(true);
}

template <>
void TableAssistant::update<SeismWell>(SeismWell const *const well) {
  assert(ForWells == _mode);

  _table->setSortingEnabled(false);

  auto &uuid = well->getUuid();
  for (int row = 0; row < _table->rowCount(); ++row) {
    if (uuid == _table->item(row, 0)->data(Qt::DisplayRole).toUuid()) {
      _table->item(row, 1)->setData(Qt::DisplayRole, well->getName());
      _table->item(row, 2)->setData(Qt::DisplayRole, well->getPointsAmount());
      break;
    }
  }

  _table->resizeColumnsToContents();
  _table->setSortingEnabled(true);
}
// Wells end

// Receivers
void TableAssistant::forReceivers() {
  assert(ForReceivers == _mode);

  _table->setColumnCount(7);
  _table->setColumnHidden(0, true);

  _table->setHorizontalHeaderLabels(QStringList() << "uuid"
                                                  << "Name"
                                                  << "x"
                                                  << "y"
                                                  << "z"
                                                  << ""
                                                  << "");

  _table->resizeColumnsToContents();

  auto horizontalHeaderObjectTable = _table->horizontalHeader();
  horizontalHeaderObjectTable->setSectionResizeMode(
      6, QHeaderView::Fixed); // fixed remove-button section
  horizontalHeaderObjectTable->setSectionResizeMode(
      5, QHeaderView::Stretch); // stretching pred-last section
  horizontalHeaderObjectTable->setDefaultAlignment(Qt::AlignLeft);
}

template <>
void TableAssistant::add<SeismReceiver>(SeismReceiver const *const receiver) {
  assert(ForReceivers == _mode);

  _table->setSortingEnabled(false);

  _table->insertRow(_table->rowCount());
  int row = _table->rowCount() - 1;

  auto &uuid = receiver->getUuid();
  _table->setItem(row, 0, new QTableWidgetItem(uuid.toString()));

  _table->setItem(row, 1, new QTableWidgetItem(receiver->getName()));

  auto &location = receiver->getLocation();
  _table->setItem(row, 2,
                  new QTableWidgetItem(QString::number(
                      static_cast<double>(std::get<0>(location)))));
  _table->setItem(row, 3,
                  new QTableWidgetItem(QString::number(
                      static_cast<double>(std::get<1>(location)))));
  _table->setItem(row, 4,
                  new QTableWidgetItem(QString::number(
                      static_cast<double>(std::get<2>(location)))));

  QPushButton *removeButton = new QPushButton();
  QIcon icon(":/icons/remove.png");
  removeButton->setStyleSheet(
      "background-color:transparent; border-style: outset");
  removeButton->setIcon(icon);
  _table->setCellWidget(row, 6, removeButton);
  connect(removeButton, &QPushButton::clicked,
          [this, &uuid]() { emit removeClicked(uuid); });

  _table->resizeColumnsToContents();
  _table->setSortingEnabled(true);
}

template <>
void TableAssistant::update<SeismReceiver>(
    SeismReceiver const *const receiver) {
  assert(ForReceivers == _mode);

  _table->setSortingEnabled(false);

  auto &uuid = receiver->getUuid();
  for (int row = 0; row < _table->rowCount(); ++row) {
    if (uuid == _table->item(row, 0)->data(Qt::DisplayRole).toUuid()) {
      _table->item(row, 1)->setData(Qt::DisplayRole, receiver->getName());
      auto &location = receiver->getLocation();
      _table->item(row, 2)->setData(Qt::DisplayRole,
                                    static_cast<double>(std::get<0>(location)));
      _table->item(row, 3)->setData(Qt::DisplayRole,
                                    static_cast<double>(std::get<1>(location)));
      _table->item(row, 4)->setData(Qt::DisplayRole,
                                    static_cast<double>(std::get<2>(location)));
      break;
    }
  }

  _table->resizeColumnsToContents();
  _table->setSortingEnabled(true);
}
// Receivers end

// Receiver-Channels
void TableAssistant::forReceiverChannels() {
  assert(ForReceiverChannels == _mode);

  _table->setColumnCount(11);

  _table->setHorizontalHeaderLabels(QStringList() << "Name"
                                                  << "Channel-Num"
                                                  << "Axis-Num"
                                                  << "N - Orientation"
                                                  << "E - Orientation"
                                                  << "D -Orientation"
                                                  << "Motion"
                                                  << "P-Station-Correction"
                                                  << "S-Station-Correction"
                                                  << "Well-ChannelNum"
                                                  << "");

  _table->resizeColumnsToContents();

  auto horizontalHeaderObjectTable = _table->horizontalHeader();
  horizontalHeaderObjectTable->setSectionResizeMode(
      10, QHeaderView::Stretch); // stretching pred-last section
  horizontalHeaderObjectTable->setDefaultAlignment(Qt::AlignLeft);
}

template <>
void TableAssistant::add<SeismChannelReceiver>(
    SeismChannelReceiver const *const channel) {
  assert(ForReceiverChannels == _mode);

  _table->setSortingEnabled(false);

  _table->insertRow(_table->rowCount());
  int row = _table->rowCount() - 1;

  _table->setItem(row, 0, new QTableWidgetItem(channel->getName()));
  _table->setItem(row, 1, new QTableWidgetItem(channel->getChannelNum()));
  _table->setItem(row, 2, new QTableWidgetItem(channel->getAxisNum()));
  auto &orientation = channel->getOrientation();
  _table->setItem(row, 3,
                  new QTableWidgetItem(QString::number(
                      static_cast<double>(std::get<0>(orientation)))));
  _table->setItem(row, 4,
                  new QTableWidgetItem(QString::number(
                      static_cast<double>(std::get<1>(orientation)))));
  _table->setItem(row, 5,
                  new QTableWidgetItem(QString::number(
                      static_cast<double>(std::get<2>(orientation)))));
  _table->setItem(row, 6, new QTableWidgetItem(channel->getMotion()));
  _table->setItem(row, 7,
                  new QTableWidgetItem(channel->getPStationCorrection()));
  _table->setItem(row, 8,
                  new QTableWidgetItem(channel->getSStationCorrection()));
  _table->setItem(row, 9, new QTableWidgetItem(channel->getWellChannelNum()));

  _table->resizeColumnsToContents();
  _table->setSortingEnabled(true);
}
// Receiver-Channels end
