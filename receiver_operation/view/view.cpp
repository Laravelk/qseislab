#include "view.h"

#include "data/seismreceiver.h"

#include <QBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismReceiver SeismReceiver;

namespace ReceiverOperation {
const int View::remove_receiver_col = 4;

View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _totalChannelNumLabel(new QLabel("0")),
      _receiversTable(new QTableWidget(this)),
      _saveButton(new QPushButton("Save")) {

  // Setting`s
  setWindowTitle("Receivers");
  setMinimumWidth(440);

  initReceiversTable(_receiversTable);

  QPushButton *fromCsvButton = new QPushButton("From CSV");
  QPushButton *addReceiverButton = new QPushButton("Add Receiver");

  _saveButton->setDisabled(true);

  QLabel *totalNumLabel = new QLabel("Total channel number: ");
  QHBoxLayout *totalNumLayout = new QHBoxLayout();
  totalNumLayout->addWidget(totalNumLabel);
  totalNumLayout->addWidget(_totalChannelNumLabel);
  totalNumLayout->addStretch(1);

  QPushButton *cancelButton = new QPushButton("Cancel");
  // Setting`s end

  // Connecting
  connect(_receiversTable, &QTableWidget::cellDoubleClicked, this,
          &View::handleReceiverClicked);
  connect(fromCsvButton, &QPushButton::clicked, this,
          &View::handleFromCsvClicked);
  connect(addReceiverButton, &QPushButton::clicked, this,
          &View::handleAddReceiverClicked);
  connect(_saveButton, &QPushButton::clicked, this, &View::accept);
  connect(cancelButton, &QPushButton::clicked, this, &View::reject);
  // Connecting end

  // Layout`s
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(fromCsvButton);
  buttonLayout->addWidget(addReceiverButton);
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(_saveButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addLayout(totalNumLayout);
  mainLayout->addWidget(_receiversTable);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void View::addReceiver(const std::unique_ptr<SeismReceiver> &receiver) {
  insertReceiverInTable(receiver);
  _totalChannelNumLabel->setNum(_totalChannelNumLabel->text().toInt() +
                                receiver->getChannelNum());
}

void View::handleReceiverClicked(int row, int col) {
  QUuid uuid = _receiversTable->item(row, 0)->text();

  if (remove_receiver_col == col) {
    emit removeReceiverClicked(uuid);
    return;
  }
}

void View::finishReceiverManager(int result) {
  if (QDialog::Accepted == result) {
    emit addReceiverClicked();
  }
}

void View::removeReceiver(const QUuid &uuid) {
  const QString str_uuid = uuid.toString();
  for (int row = 0; row < _receiversTable->rowCount(); ++row) {
    if (str_uuid == _receiversTable->item(row, 0)->text()) {
      _totalChannelNumLabel->setNum(
          _totalChannelNumLabel->text().toInt() -
          _receiversTable->item(row, 3)->text().toInt());

      _receiversTable->removeRow(row);
      return;
    }
  }
}

void View::settingReceiverInfo(const std::unique_ptr<SeismReceiver> &horizon) {
  _addReceiverManager->settingReceiverInfo(horizon);
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

void View::initReceiversTable(QTableWidget *table) {
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setColumnCount(5);

  // configure column settings
  table->setHorizontalHeaderLabels(QStringList() << "uuid"
                                                 << "Name"
                                                 << "Location"
                                                 << "Channel Number"
                                                 << "Remove");
  //  table->setHorizontalHeaderItem(0, new QTableWidgetItem("uuid"));
  table->setColumnHidden(0, true);
  //  table->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
  //  table->setHorizontalHeaderItem(2, new QTableWidgetItem("Location"));
  //  table->setHorizontalHeaderItem(3, new QTableWidgetItem("Channel Number"));
  //  table->setHorizontalHeaderItem(remove_receiver_col, new
  //  QTableWidgetItem("Remove"));
}

void View::insertReceiverInTable(
    const std::unique_ptr<SeismReceiver> &receiver) {
  _receiversTable->insertRow(_receiversTable->rowCount());

  int insertRow = _receiversTable->rowCount() - 1;

  _receiversTable->setItem(
      insertRow, 0, new QTableWidgetItem(receiver->getUuid().toString()));

  _receiversTable->setItem(insertRow, 1,
                           new QTableWidgetItem(receiver->getName()));

  _receiversTable->setItem(
      insertRow, 2,
      new QTableWidgetItem(
          QString("%1 %2 %3")
              .arg(static_cast<double>(std::get<0>(receiver->getLocation())), 0,
                   'f', 2)
              .arg(static_cast<double>(std::get<1>(receiver->getLocation())), 0,
                   'f', 2)
              .arg(static_cast<double>(std::get<2>(receiver->getLocation())), 0,
                   'f', 2)));

  _receiversTable->setItem(
      insertRow, 3,
      new QTableWidgetItem(QString::number(receiver->getChannelNum())));

  QTableWidgetItem *removeItem = new QTableWidgetItem("Remove");
  removeItem->setTextAlignment(Qt::AlignCenter);
  removeItem->setBackground(Qt::red);
  _receiversTable->setItem(insertRow, remove_receiver_col, removeItem);
}

void View::handleFromCsvClicked() {
  if (0 != _receiversTable->rowCount()) {
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText("There are receivers in the project");
    msgBox->setInformativeText("Delete receivers?");
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                               QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Cancel);

    int ret = msgBox->exec();
    switch (ret) {
    case QMessageBox::Cancel:
      return;
    case QMessageBox::No:
      break;
    case QMessageBox::Yes:
      const int rowCount = _receiversTable->rowCount();
      for (int row = 0; row < rowCount; ++row) {
        handleReceiverClicked(0, remove_receiver_col);
      }
      break;
    }
  }

  QFileDialog *fileDialog = new QFileDialog(this);
  fileDialog->setFileMode(QFileDialog::ExistingFile);
  fileDialog->setOption(QFileDialog::DontResolveSymlinks);
  fileDialog->setNameFilter("*.csv");

  connect(fileDialog, &QFileDialog::fileSelected,
          [this](auto &path) { emit sendCsvFilePath(path); });
  fileDialog->open();
}

void View::handleAddReceiverClicked() {
  _addReceiverManager = std::make_unique<AddReceiverManager>(this);
  connect(_addReceiverManager.get(), &AddReceiverManager::notify, this,
          &View::setNotification);
  connect(_addReceiverManager.get(), &AddReceiverManager::finished, this,
          &View::finishReceiverManager);

  _addReceiverManager->setModal(true);
  _addReceiverManager->show();
}

} // namespace ReceiverOperation
