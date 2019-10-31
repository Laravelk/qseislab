#include "tableassistant.h"

#include "data/seismhorizon.h"
#include "data/seismreceiver.h"
#include "data/seismwell.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QPushButton>

#include <assert.h>

typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismWell SeismWell;
typedef Data::SeismReceiver SeismReceiver;

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

void TableAssistant::requestRemoveAll() {
  QList<QUuid> list;
  for (int row = 0; row < _table->rowCount(); ++row) {
    list.append(_table->item(row, 0)->data(Qt::DisplayRole).toUuid());
  }

  for (auto &uuid : list) {
    emit removeClicked(uuid);
  }
}

template <typename T>
void TableAssistant::setAll(const std::map<QUuid, std::unique_ptr<T>> &map) {
  clearTable();

  for (auto &uuid_obj : map) {
    add<T>(uuid_obj.second);
  }
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
void TableAssistant::add<SeismHorizon>(
    const std::unique_ptr<SeismHorizon> &horizon) {
  assert(ForHorizons == _mode);
  _table->setSortingEnabled(false);

  _table->insertRow(_table->rowCount());
  int row = _table->rowCount() - 1;

  auto uuid = horizon->getUuid();
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
  QIcon icon(":/remove_button.png");
  removeButton->setStyleSheet("background-color:white; border-style: outset");
  removeButton->setIcon(icon);
  _table->setCellWidget(row, 6, removeButton);
  connect(removeButton, &QPushButton::clicked,
          [this, uuid]() { emit removeClicked(uuid); });

  _table->resizeColumnsToContents();
  _table->setSortingEnabled(true);
}

template <>
void TableAssistant::update<SeismHorizon>(
    const std::unique_ptr<SeismHorizon> &horizon) {
  assert(ForHorizons == _mode);
  _table->setSortingEnabled(false);

  const auto &uuid = horizon->getUuid();
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

template <>
void TableAssistant::add<SeismWell>(const std::unique_ptr<SeismWell> &well) {
  assert(ForWells == _mode);
  _table->setSortingEnabled(false);

  _table->insertRow(_table->rowCount());
  int row = _table->rowCount() - 1;

  auto uuid = well->getUuid();
  _table->setItem(row, 0, new QTableWidgetItem(uuid.toString()));

  _table->setItem(row, 1, new QTableWidgetItem(well->getName()));

  _table->setItem(
      row, 2, new QTableWidgetItem(QString::number(well->getPointsAmount())));

  QPushButton *removeButton = new QPushButton();
  QIcon icon(":/remove_button.png");
  removeButton->setStyleSheet("background-color:white; border-style: outset");
  removeButton->setIcon(icon);
  _table->setCellWidget(row, 4, removeButton);
  connect(removeButton, &QPushButton::clicked,
          [this, uuid]() { emit removeClicked(uuid); });

  _table->resizeColumnsToContents();
  _table->setSortingEnabled(true);
}

template <>
void TableAssistant::update<SeismWell>(const std::unique_ptr<SeismWell> &well) {
  assert(ForWells == _mode);
  _table->setSortingEnabled(false);

  const auto &uuid = well->getUuid();
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
void TableAssistant::add<SeismReceiver>(
    const std::unique_ptr<SeismReceiver> &receiver) {
  assert(ForReceivers == _mode);
  _table->setSortingEnabled(false);

  _table->insertRow(_table->rowCount());
  int row = _table->rowCount() - 1;

  auto uuid = receiver->getUuid();
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
  QIcon icon(":/remove_button.png");
  removeButton->setStyleSheet("background-color:white; border-style: outset");
  removeButton->setIcon(icon);
  _table->setCellWidget(row, 6, removeButton);
  connect(removeButton, &QPushButton::clicked,
          [this, uuid]() { emit removeClicked(uuid); });

  _table->resizeColumnsToContents();
  _table->setSortingEnabled(true);
}

template <>
void TableAssistant::update<SeismReceiver>(
    const std::unique_ptr<SeismReceiver> &receiver) {
  assert(ForReceivers == _mode);
  _table->setSortingEnabled(false);

  const auto &uuid = receiver->getUuid();
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
