#include "addreceivermanager.h"

#include "data/seismreceiver.h"

#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

typedef Data::SeismReceiver SeismReceiver;

namespace ReceiverOperation {
AddReceiverManager::AddReceiverManager(
    const std::map<QUuid, QString> &wellNames_map, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _nameLineEdit(new QLineEdit()),
      //    _channelNumLineEdit(new QLineEdit()),
      _X_LineEdit(new QLineEdit()), _Y_LineEdit(new QLineEdit()),
      _Z_LineEdit(new QLineEdit()), _comboBox(new QComboBox()) {

  // Setting`s
  setWindowTitle("Add Receiver");

  _comboBox->addItem("Select Owner_Array...");
  for (auto &uuid_name : wellNames_map) {
    _comboBox->addItem(uuid_name.second, uuid_name.first);
  }

  QPushButton *addButton = new QPushButton("Add");

  QPushButton *cancelButton = new QPushButton("Cancel");
  // Setting`s end

  // Connecting
  connect(addButton, &QPushButton::clicked, this,
          &AddReceiverManager::handleAccept);
  connect(cancelButton, &QPushButton::clicked, this,
          &AddReceiverManager::reject);
  // Connecting end

  // Layout`s
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(cancelButton);

  QFormLayout *formLayout = new QFormLayout();
  formLayout->addRow("Name: ", _nameLineEdit);
  formLayout->addRow("X: ", _X_LineEdit);
  formLayout->addRow("Y: ", _Y_LineEdit);
  formLayout->addRow("Z: ", _Z_LineEdit);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addLayout(formLayout);
  mainLayout->addWidget(_comboBox);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  // Layout`s end
}

const QUuid
AddReceiverManager::settingReceiverInfo(SeismReceiver *const receiver) {
  receiver->setName(_nameLineEdit->text());
  receiver->setLocation({_X_LineEdit->text().toFloat(),
                         _Y_LineEdit->text().toFloat(),
                         _Z_LineEdit->text().toFloat()});

  return _comboBox->currentData().toUuid();
}

void AddReceiverManager::handleAccept() {
  QString err_msg;
  if (_nameLineEdit->text().isEmpty()) {
    err_msg += "Name-field is empty\n";
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
  if (0 == _comboBox->currentIndex()) {
    err_msg += "Well-name isn`t selected\n";
  }

  if (err_msg.isEmpty()) {
    accept();
  } else {
    emit notify(err_msg);
  }
}

} // namespace ReceiverOperation
