#include "addrecievermanager.h"

#include "data/seismreciever.h"

#include <QFormLayout>
#include <QPushButton>

namespace RecieverOperation {

AddRecieverManager::AddRecieverManager(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _nameLineEdit(new QLineEdit()), _channelNumLineEdit(new QLineEdit()),
      _X_LineEdit(new QLineEdit()), _Y_LineEdit(new QLineEdit()),
      _Z_LineEdit(new QLineEdit()) {

  setWindowTitle("Add Reciever");

  QPushButton *addButton = new QPushButton("Add");
  connect(addButton, SIGNAL(clicked()), this, SLOT(handleAccept()));

  QPushButton *cancelButton = new QPushButton("Cancel");
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(cancelButton);

  QFormLayout *formLayout = new QFormLayout();
  formLayout->addRow("Name: ", _nameLineEdit);
  formLayout->addRow("Channel Num: ", _channelNumLineEdit);
  formLayout->addRow("X: ", _X_LineEdit);
  formLayout->addRow("Y: ", _Y_LineEdit);
  formLayout->addRow("Z: ", _Z_LineEdit);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addLayout(formLayout);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
}

void AddRecieverManager::settingRecieverInfo(
    const std::unique_ptr<Data::SeismReciever> &reciever) {
  reciever->setName(_nameLineEdit->text());
  reciever->setChannelNum(_channelNumLineEdit->text().toInt());
  reciever->setLocation({_X_LineEdit->text().toFloat(),
                         _Y_LineEdit->text().toFloat(),
                         _Z_LineEdit->text().toFloat()});
}

void AddRecieverManager::handleAccept() {
  QString err_msg;
  if (_nameLineEdit->text().isEmpty()) {
    err_msg += "Name-field is empty\n";
  }
  if (_channelNumLineEdit->text().isEmpty()) {
    err_msg += "ChannelNum-field is empty\n";
  }
  if (_X_LineEdit->text().isEmpty()) {
    err_msg += "X-coordinate-field is empty\n";
  }
  if (_Y_LineEdit->text().isEmpty()) {
    err_msg += "Y-coordinate-field is empty\n";
  }
  if (_Z_LineEdit->text().isEmpty()) {
    err_msg += "Z-coordinate-field is empty\n";
  }

  if (err_msg.isEmpty()) {
    accept();
  } else {
    emit notify(err_msg);
  }
}

} // namespace RecieverOperation