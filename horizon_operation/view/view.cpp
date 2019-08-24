#include "view.h"

#include "data/seismhorizon.h"

#include <QBoxLayout>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismHorizon SeismHorizon;

namespace HorizonOperation {
View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _horizonsTable(new QTableWidget(this)),
      _saveButton(new QPushButton("Save")) {

  setWindowTitle("Horizons");
  setMinimumWidth(440);

  initHorizonsTable(_horizonsTable);
  connect(_horizonsTable, SIGNAL(cellDoubleClicked(int, int)), this,
          SLOT(handleHorizonClicked(int, int)));

  QPushButton *addHorizonButton = new QPushButton("Add Horizon");
  connect(addHorizonButton, SIGNAL(clicked()), this,
          SLOT(handleAddHorizonClicked()));

  _saveButton->setDisabled(true);
  connect(_saveButton, SIGNAL(clicked()), this, SLOT(accept()));

  QPushButton *cancelButton = new QPushButton("Cancel");
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

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

void View::finishHorizonManager(int result) {
  if (QDialog::Accepted == result) {
    emit addHorizonClicked();
  }
}

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
  table->setColumnCount(6);

  // configure column settings
  table->setHorizontalHeaderItem(0, new QTableWidgetItem("uuid"));
  table->setColumnHidden(0, true);
  table->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
  table->setHorizontalHeaderItem(2, new QTableWidgetItem("Point Number"));
  table->setHorizontalHeaderItem(3, new QTableWidgetItem("Nx"));
  table->setColumnWidth(3, 50);
  table->setHorizontalHeaderItem(4, new QTableWidgetItem("Ny"));
  table->setColumnWidth(4, 50);
  table->setHorizontalHeaderItem(5, new QTableWidgetItem("Remove"));
}

void View::insertHorizonInTable(const std::unique_ptr<SeismHorizon> &horizon) {
  _horizonsTable->insertRow(_horizonsTable->rowCount());

  int insertRow = _horizonsTable->rowCount() - 1;

  _horizonsTable->setItem(insertRow, 0,
                          new QTableWidgetItem(horizon->getUuid().toString()));

  _horizonsTable->setItem(insertRow, 1,
                          new QTableWidgetItem(horizon->getName()));

  _horizonsTable->setItem(
      insertRow, 2,
      new QTableWidgetItem(QString::number(horizon->getPointsNumber())));

  _horizonsTable->setItem(
      insertRow, 3, new QTableWidgetItem(QString::number(horizon->getNx())));

  _horizonsTable->setItem(
      insertRow, 4, new QTableWidgetItem(QString::number(horizon->getNy())));

  QTableWidgetItem *removeItem = new QTableWidgetItem("Remove");
  removeItem->setTextAlignment(Qt::AlignCenter);
  removeItem->setBackground(Qt::red);
  _horizonsTable->setItem(insertRow, 5, removeItem);
}

void View::handleAddHorizonClicked() {
  _addHorizonManager = std::make_unique<AddHorizonManager>(this);

  connect(_addHorizonManager.get(), SIGNAL(sendFilePath(const QString &)), this,
          SLOT(recvFilePath(const QString &)));
  connect(_addHorizonManager.get(), SIGNAL(notify(const QString &)), this,
          SLOT(setNotification(const QString &)));
  connect(_addHorizonManager.get(), SIGNAL(finished(int)), this,
          SLOT(finishHorizonManager(int)));

  _addHorizonManager->setModal(true);
  _addHorizonManager->show();
}

void View::recvFilePath(const QString &filePath) {
  emit sendFilePath(filePath);
}

} // namespace HorizonOperation
