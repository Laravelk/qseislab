
#include "rotatedatasettingdialog.h"

#include "data/projectsettings.h"

#include <Eigen/Dense>

#include <iostream> // TODO: remove!

namespace ProjectOperation {
RotateDataSettingDialog::RotateDataSettingDialog(QWidget *parent)
    : SettingDialog(parent), _toEBasisButton(new QRadioButton("To E-basis")),
      _toReceiverBasisButton(new QRadioButton("To Receiver-basis")) {

  // Setting`s
  setWindowTitle("Rotate Data Settings");
  // Setting`s end

  // Connecting
  connect(_toEBasisButton, &QRadioButton::clicked,
          [this] { this->hasChanged(true); });
  connect(_toReceiverBasisButton, &QRadioButton::clicked,
          [this] { this->hasChanged(true); });
  // Connecting end

  // Layout`s
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_toEBasisButton);
  mainLayout->addWidget(_toReceiverBasisButton);
  mainLayout->addStretch(1);
  mainLayout->addLayout(_buttonsLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void RotateDataSettingDialog::update(
    Data::ProjectSettings const *const projectSettings) {

  switch (projectSettings->getRotateDataParameters().getMode()) {
  case RotateData::Parameters::Mode::EBASIS:
    _toEBasisButton->setChecked(true);
    break;
  case RotateData::Parameters::Mode::RECEIVERS:
    _toReceiverBasisButton->setChecked(true);
    break;
  case RotateData::Parameters::Mode::INDEFINITE:
    // TODO: implement!
    break;
  }

  this->hasChanged(false);
}

void RotateDataSettingDialog::setSettings(
    Data::ProjectSettings *const projectSettings) {

  if (_toEBasisButton->isChecked()) {
    projectSettings->getRotateDataParameters().setMode(
        RotateData::Parameters::Mode::EBASIS);
  }
  if (_toReceiverBasisButton->isChecked()) {
    projectSettings->getRotateDataParameters().setMode(
        RotateData::Parameters::Mode::RECEIVERS);
  }
}
} // namespace ProjectOperation
