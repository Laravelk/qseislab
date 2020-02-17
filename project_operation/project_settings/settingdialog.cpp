#include "settingdialog.h"

namespace ProjectOperation {
SettingDialog::SettingDialog(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _buttonsLayout(new QHBoxLayout()), _okButton(new QPushButton("Ok")),
      _applyButton(new QPushButton("Apply")),
      _cancelButton(new QPushButton("Cancel")) {

  _applyButton->setDisabled(true);

  connect(_okButton, &QPushButton::clicked, [this] {
    emit apply();
    this->done(QDialog::Accepted);
  });
  connect(_applyButton, &QPushButton::clicked, [this] {
    _applyButton->setDisabled(true);
    emit apply();
  });
  connect(_cancelButton, &QPushButton::clicked,
          [this] { this->done(QDialog::Rejected); });

  _buttonsLayout->addStretch(1);
  _buttonsLayout->addWidget(_okButton);
  _buttonsLayout->addWidget(_applyButton);
  _buttonsLayout->addWidget(_cancelButton);
}

void SettingDialog::hasChanged(bool b) { _applyButton->setEnabled(b); }
} // namespace ProjectOperation
