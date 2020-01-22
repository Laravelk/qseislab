#include "settingdialog.h"

#include "project_operation/project_settings/ffilteringdatasettingdialog.h"
#include "project_operation/project_settings/rotatedatasettingdialog.h"
#include "project_operation/project_settings/testmultipliersettingdialog.h"

namespace ProjectOperation {
SettingDialog::SettingDialog(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _buttonsLayout(new QHBoxLayout()), _okButton(new QPushButton("Ok")),
      _applyButton(new QPushButton("Apply")),
      _cancelButton(new QPushButton("Cancel")) {

  _applyButton->setDisabled(true);

  connect(_okButton, &QPushButton::clicked, [this] {
    if (_applyButton->isEnabled()) {
      emit apply();
    }
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

SettingDialog *getSettingDialog(Data::SeismEvent::TransformOperation oper) {
  switch (oper) {
  case Data::SeismEvent::TransformOperation::TestMultiplier:
    return new ProjectOperation::TestMultiplierSettingDialog();
  case Data::SeismEvent::TransformOperation::RotateData:
    return new ProjectOperation::RotateDataSettingDialog();
  case Data::SeismEvent::TransformOperation::FFilteringData:
    return new ProjectOperation::FFilteringDataSettingDialog();
  }

  assert(false & "unsupported setting dialog");
  return nullptr;
}

} // namespace ProjectOperation
