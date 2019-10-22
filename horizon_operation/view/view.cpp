#include "view.h"

#include "data/seismhorizon.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismHorizon SeismHorizon;

namespace HorizonOperation {
View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _horizonsTable(new QTableWidget(this)),
      _saveButton(new QPushButton("Save")) {

  // Setting`s
  setWindowTitle("Horizons");
  setMinimumWidth(440);

  initHorizonsTable(_horizonsTable);

  QPushButton *addHorizonButton = new QPushButton("Add Horizon");

  _saveButton->setDisabled(true);

  QPushButton *cancelButton = new QPushButton("Cancel");
  // Setting`s end

  // Connecting
  connect(_horizonsTable, &QTableWidget::cellDoubleClicked, this,
          &View::handleHorizonClicked);
  connect(addHorizonButton, &QPushButton::clicked, this,
          &View::handleAddHorizonClicked);
  connect(_saveButton, &QPushButton::clicked, this, &View::accept);
  connect(cancelButton, &QPushButton::clicked, this, &View::reject);
  // Connecting end

  // Layout`s
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(addHorizonButton);
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(_saveButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_horizonsTable);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void View::addHorizon(const std::unique_ptr<SeismHorizon> &horizon) {
  insertHorizonInTable(horizon);
}

void View::updateHorizon(const std::unique_ptr<SeismHorizon> &horizon) {
  _addHorizonManager->update(horizon);
}

void View::handleHorizonClicked(int row, int col) {
  QUuid uuid = _horizonsTable->item(row, 0)->text();

  if (5 == col) {
    emit removeHorizonClicked(uuid);
    return;
  }
}

// void View::finishHorizonManager(int result) {
//  if (QDialog::Accepted == result) {
//    emit addHorizonClicked();
//  }
//}

void View::removeHorizon(const QUuid &uuid) {
  const QString str_uuid = uuid.toString();
  for (int row = 0; row < _horizonsTable->rowCount(); ++row) {
    if (str_uuid == _horizonsTable->item(row, 0)->text()) {
      _horizonsTable->removeRow(row);
      return;
    }
  }
}

void View::settingHorizonInfo(
    const std::unique_ptr<Data::SeismHorizon> &horizon) {
  _addHorizonManager->settingHorizonInfo(horizon);
}

void View::setNotification(const QString &text) {
  QMessageBox *msg = new QMessageBox(QMessageBox::Critical, "Message", text,
                                     QMessageBox::Ok, this);
  msg->show();
}

void View::changed(bool b) {
  _saveButton->setEnabled(b);
  _saveButton->setFocus();
}

void View::initHorizonsTable(QTableWidget *table) {
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setColumnCount(7);
  table->setColumnHidden(0, true);

  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  table->setSelectionMode(QAbstractItemView::SingleSelection);

  // configure column settings
  //  table->setHorizontalHeaderItem(0, new QTableWidgetItem("uuid"));
  //  table->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
  //  table->setHorizontalHeaderItem(2, new QTableWidgetItem("Point Number"));
  //  table->setHorizontalHeaderItem(3, new QTableWidgetItem("Nx"));
  //  table->setColumnWidth(3, 50);
  //  table->setHorizontalHeaderItem(4, new QTableWidgetItem("Ny"));
  //  table->setColumnWidth(4, 50);
  //  table->setHorizontalHeaderItem(5, new QTableWidgetItem("Remove"));

  table->setHorizontalHeaderLabels(QStringList() << "uuid"
                                                 << "Name"
                                                 << "Point Number"
                                                 << "Nx"
                                                 << "Ny"
                                                 << ""
                                                 << "");

  table->resizeColumnsToContents();

  auto horizontalHeaderObjectTable = table->horizontalHeader();
  horizontalHeaderObjectTable->setSectionResizeMode(
      6, QHeaderView::Fixed); // fixed remove-button section
  horizontalHeaderObjectTable->setSectionResizeMode(
      5, QHeaderView::Stretch); // stretching pred-last section
  horizontalHeaderObjectTable->setDefaultAlignment(Qt::AlignLeft);
  table->verticalHeader()->setVisible(false);
}

void View::insertHorizonInTable(const std::unique_ptr<SeismHorizon> &horizon) {
  _horizonsTable->insertRow(_horizonsTable->rowCount());

  int insertRow = _horizonsTable->rowCount() - 1;

  auto &uuid = horizon->getUuid();
  _horizonsTable->setItem(insertRow, 0, new QTableWidgetItem(uuid.toString()));

  _horizonsTable->setItem(insertRow, 1,
                          new QTableWidgetItem(horizon->getName()));

  _horizonsTable->setItem(
      insertRow, 2,
      new QTableWidgetItem(QString::number(horizon->getPointsNumber())));

  _horizonsTable->setItem(
      insertRow, 3, new QTableWidgetItem(QString::number(horizon->getNx())));

  _horizonsTable->setItem(
      insertRow, 4, new QTableWidgetItem(QString::number(horizon->getNy())));

  QPushButton *removeButton = new QPushButton();
  QIcon icon(":/remove_button.png");
  removeButton->setStyleSheet("background-color:white; border-style: outset");
  removeButton->setIcon(icon);
  _horizonsTable->setCellWidget(insertRow, 6, removeButton);
  connect(removeButton, &QPushButton::clicked,
          [this, uuid]() { emit removeHorizonClicked(uuid); });

  //  _horizonsTable->resizeColumnsToContents();

  //  QTableWidgetItem *removeItem = new QTableWidgetItem("Remove");
  //  removeItem->setTextAlignment(Qt::AlignCenter);
  //  removeItem->setBackground(Qt::red);
  //  _horizonsTable->setItem(insertRow, 5, removeItem);
}

void View::handleAddHorizonClicked() {
  _addHorizonManager = std::make_unique<AddHorizonManager>(this);

  connect(_addHorizonManager.get(), &AddHorizonManager::sendFilePath,
          [this](auto &path) { emit sendFilePath(path); });
  connect(_addHorizonManager.get(), &AddHorizonManager::notify, this,
          &View::setNotification);
  connect(_addHorizonManager.get(), &AddHorizonManager::finished,
          [this](int res) {
            if (QDialog::Accepted == res) {
              emit addHorizonClicked();
            };
          });

  _addHorizonManager->setModal(true);
  _addHorizonManager->show();
}

// void View::recvFilePath(const QString &filePath) {
//  emit sendFilePath(filePath);
//}

} // namespace HorizonOperation
