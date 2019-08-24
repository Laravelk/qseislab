#include "view.h"

#include "data/seismreciever.h"

#include <QBoxLayout>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismReciever SeismReciever;

namespace RecieverOperation {
View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _totalChannelNumLabel(new QLabel("0")),
      _recieversTable(new QTableWidget(this)),
      _saveButton(new QPushButton("Save")) {

  setWindowTitle("Recievers");
  setMinimumWidth(440);

  initRecieversTable(_recieversTable);
  connect(_recieversTable, SIGNAL(cellDoubleClicked(int, int)), this,
          SLOT(handleRecieverClicked(int, int)));

  QPushButton *addRecieverButton = new QPushButton("Add Reciever");
  connect(addRecieverButton, SIGNAL(clicked()), this,
          SLOT(handleAddRecieverClicked()));

  _saveButton->setDisabled(true);
  connect(_saveButton, SIGNAL(clicked()), this, SLOT(accept()));

  QLabel *totalNumLabel = new QLabel("Total channel number: ");
  QHBoxLayout *totalNumLayout = new QHBoxLayout();
  totalNumLayout->addWidget(totalNumLabel);
  totalNumLayout->addWidget(_totalChannelNumLabel);
  totalNumLayout->addStretch(1);

  QPushButton *cancelButton = new QPushButton("Cancel");
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(addRecieverButton);
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(_saveButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addLayout(totalNumLayout);
  mainLayout->addWidget(_recieversTable);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
}

void View::addReciever(const std::unique_ptr<SeismReciever> &reciever) {
  insertRecieverInTable(reciever);
  _totalChannelNumLabel->setNum(_totalChannelNumLabel->text().toInt() +
                                reciever->getChannelNum());
}

void View::handleRecieverClicked(int row, int col) {
  QUuid uuid = _recieversTable->item(row, 0)->text();

  if (4 == col) {
    emit removeRecieverClicked(uuid);
    return;
  }
}

void View::finishRecieverManager(int result) {
  if (QDialog::Accepted == result) {
    emit addRecieverClicked();
  }
}

void View::removeReciever(const QUuid &uuid) {
  const QString str_uuid = uuid.toString();
  for (int row = 0; row < _recieversTable->rowCount(); ++row) {
    if (str_uuid == _recieversTable->item(row, 0)->text()) {
      _totalChannelNumLabel->setNum(
          _totalChannelNumLabel->text().toInt() -
          _recieversTable->item(row, 3)->text().toInt());

      _recieversTable->removeRow(row);
      return;
    }
  }
}

void View::settingRecieverInfo(const std::unique_ptr<SeismReciever> &horizon) {
  _addRecieverManager->settingRecieverInfo(horizon);
}

void View::changed(bool b) {
  _saveButton->setEnabled(b);
  _saveButton->setFocus();
}

void View::setNotification(const QString &text) {
  QMessageBox *msg = new QMessageBox(QMessageBox::Critical, "Message", text,
                                     QMessageBox::Ok, this);
  msg->show();
}

void View::initRecieversTable(QTableWidget *table) {
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setColumnCount(5);

  // configure column settings
  table->setHorizontalHeaderItem(0, new QTableWidgetItem("uuid"));
  table->setColumnHidden(0, true);
  table->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
  table->setHorizontalHeaderItem(2, new QTableWidgetItem("Location"));
  table->setHorizontalHeaderItem(3, new QTableWidgetItem("Channel Number"));
  table->setHorizontalHeaderItem(4, new QTableWidgetItem("Remove"));
}

void View::insertRecieverInTable(
    const std::unique_ptr<SeismReciever> &reciever) {
  _recieversTable->insertRow(_recieversTable->rowCount());

  int insertRow = _recieversTable->rowCount() - 1;

  _recieversTable->setItem(
      insertRow, 0, new QTableWidgetItem(reciever->getUuid().toString()));

  _recieversTable->setItem(insertRow, 1,
                           new QTableWidgetItem(reciever->getName()));

  _recieversTable->setItem(
      insertRow, 2,
      new QTableWidgetItem(
          QString("%1 %2 %3")
              .arg(static_cast<double>(std::get<0>(reciever->getLocation())), 0,
                   'f', 2)
              .arg(static_cast<double>(std::get<1>(reciever->getLocation())), 0,
                   'f', 2)
              .arg(static_cast<double>(std::get<2>(reciever->getLocation())), 0,
                   'f', 2)));

  _recieversTable->setItem(
      insertRow, 3,
      new QTableWidgetItem(QString::number(reciever->getChannelNum())));

  QTableWidgetItem *removeItem = new QTableWidgetItem("Remove");
  removeItem->setTextAlignment(Qt::AlignCenter);
  removeItem->setBackground(Qt::red);
  _recieversTable->setItem(insertRow, 4, removeItem);
}

void View::handleAddRecieverClicked() {
  _addRecieverManager = std::make_unique<AddRecieverManager>(this);
  connect(_addRecieverManager.get(), SIGNAL(notify(const QString &)), this,
          SLOT(setNotification(const QString &)));
  connect(_addRecieverManager.get(), SIGNAL(finished(int)), this,
          SLOT(finishRecieverManager(int)));

  _addRecieverManager->setModal(true);
  _addRecieverManager->show();
}

} // namespace RecieverOperation
