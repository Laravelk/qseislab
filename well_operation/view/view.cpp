#include "view.h"

#include "data/seismwell.h"

#include <QBoxLayout>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismWell SeismWell;

namespace WellOperation {
View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _wellsTable(new QTableWidget(this)),
      _saveButton(new QPushButton("Save")) {

  // Setting`s
  setWindowTitle("Wells");
  setMinimumWidth(440);

  initWellsTable(_wellsTable);

  QPushButton *addWellButton = new QPushButton("Add Well");

  _saveButton->setDisabled(true);

  QPushButton *cancelButton = new QPushButton("Cancel");
  // Setting`s end

  // Connecting
  connect(_wellsTable, &QTableWidget::cellDoubleClicked, this,
          &View::handleWellClicked);
  connect(addWellButton, &QPushButton::clicked, this,
          &View::handleAddWellClicked);
  connect(_saveButton, &QPushButton::clicked, this, &View::accept);
  connect(cancelButton, &QPushButton::clicked, this, &View::reject);
  // Connecting end

  // Layout`s
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(addWellButton);
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(_saveButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_wellsTable);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void View::addWell(const std::unique_ptr<SeismWell> &well) {
  insertWellInTable(well);
}

void View::updateWell(const std::unique_ptr<SeismWell> &well) {
  _addWellManager->update(well);
}

void View::handleWellClicked(int row, int col) {
  QUuid uuid = _wellsTable->item(row, 0)->text();

  if (3 == col) {
    emit removeWellClicked(uuid);
    return;
  }
}

void View::removeWell(const QUuid &uuid) {
  const QString str_uuid = uuid.toString();
  for (int row = 0; row < _wellsTable->rowCount(); ++row) {
    if (str_uuid == _wellsTable->item(row, 0)->text()) {
      _wellsTable->removeRow(row);
      return;
    }
  }
}

void View::settingWellInfo(const std::unique_ptr<Data::SeismWell> &well) {
  _addWellManager->settingWellInfo(well);
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

void View::initWellsTable(QTableWidget *table) {
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setColumnCount(4);

  // configure column settings
  table->setHorizontalHeaderItem(0, new QTableWidgetItem("uuid"));
  table->setColumnHidden(0, true);
  table->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
  table->setHorizontalHeaderItem(2, new QTableWidgetItem("Point Number"));
  table->setHorizontalHeaderItem(3, new QTableWidgetItem("Remove"));
}

void View::insertWellInTable(const std::unique_ptr<SeismWell> &well) {
  _wellsTable->insertRow(_wellsTable->rowCount());

  int insertRow = _wellsTable->rowCount() - 1;

  _wellsTable->setItem(insertRow, 0,
                       new QTableWidgetItem(well->getUuid().toString()));

  _wellsTable->setItem(insertRow, 1, new QTableWidgetItem(well->getName()));

  _wellsTable->setItem(
      insertRow, 2,
      new QTableWidgetItem(QString::number(well->getPointsNumber())));

  QTableWidgetItem *removeItem = new QTableWidgetItem("Remove");
  removeItem->setTextAlignment(Qt::AlignCenter);
  removeItem->setBackground(Qt::red);
  _wellsTable->setItem(insertRow, 3, removeItem);
}

void View::handleAddWellClicked() {
  _addWellManager = std::make_unique<AddWellManager>(this);

  connect(_addWellManager.get(), &AddWellManager::sendFilePath,
          [this](auto &path) { emit sendFilePath(path); });
  connect(_addWellManager.get(), &AddWellManager::notify, this,
          &View::setNotification);
  connect(_addWellManager.get(), &AddWellManager::finished, [this](int res) {
    if (QDialog::Accepted == res) {
      emit addWellClicked();
    };
  });

  _addWellManager->setModal(true);
  _addWellManager->show();
}

// void View::recvFilePath(const QString &filePath) {
//  emit sendFilePath(filePath);
//}

} // namespace WellOperation
