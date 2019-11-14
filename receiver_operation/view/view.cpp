#include "view.h"

#include "data/seismreceiver.h"
#include "inforeceiver.h"
#include "totalchannelcounter.h"

#include <QBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismReceiver SeismReceiver;

namespace ReceiverOperation {
View::View(const std::map<QUuid, QString> &wellNames_map, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _channelCounter(new TotalChannelCounter()),
      _receiversTable(new TableAssistant(TableAssistant::ForReceivers, this)),
      _saveButton(new QPushButton("Save")), _wellNames_map(wellNames_map) {

  // Setting`s
  setWindowTitle("Receivers");
  setMinimumWidth(440);

  _saveButton->setDisabled(true);
  QPushButton *fromCsvButton = new QPushButton("From CSV");
  QPushButton *addReceiverButton = new QPushButton("Add Receiver");

  QPushButton *cancelButton = new QPushButton("Cancel");
  // Setting`s end

  // Connecting
  connect(_receiversTable, &TableAssistant::removeClicked,
          [this](auto &uuid) { emit removeReceiverClicked(uuid); });
  connect(_receiversTable, &TableAssistant::viewClicked,
          [this](auto &uuid) { emit receiverClicked(uuid); });
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
  mainLayout->addWidget(_channelCounter);
  mainLayout->addWidget(_receiversTable, 1);
  //  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void View::addReceiver(const std::shared_ptr<SeismReceiver> &receiver) {
  _receiversTable->add<SeismReceiver>(receiver);
  _channelCounter->add(receiver);
}

void View::finishReceiverManager(int result) {
  if (QDialog::Accepted == result) {
    emit addReceiverClicked();
  }
}

void View::removeReceiver(const QUuid &uuid) {
  _receiversTable->remove(uuid);
  _channelCounter->remove(uuid);
}

void View::viewFullInfo(const std::shared_ptr<Data::SeismReceiver> &receiver) {
  InfoReceiver *about = new InfoReceiver(receiver, this);
  about->setModal(true);
  about->show();
}

const QUuid
View::settingReceiverInfo(const std::shared_ptr<SeismReceiver> &receiver) {
  return _addReceiverManager->settingReceiverInfo(receiver);
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

void View::handleFromCsvClicked() {
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
    _receiversTable->requestRemoveAll();
    break;
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
  _addReceiverManager =
      std::make_unique<AddReceiverManager>(_wellNames_map, this);
  connect(_addReceiverManager.get(), &AddReceiverManager::notify, this,
          &View::setNotification);
  connect(_addReceiverManager.get(), &AddReceiverManager::finished, this,
          &View::finishReceiverManager);

  _addReceiverManager->setModal(true);
  _addReceiverManager->show();
}

} // namespace ReceiverOperation
