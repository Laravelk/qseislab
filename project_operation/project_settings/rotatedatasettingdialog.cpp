#include "rotatedatasettingdialog.h"

#include "data/projectsettings.h"

#include <Eigen/Dense>

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
    const Data::ProjectSettings &projectSettings) {
  _toEBasisButton->setDown(false);
  _toReceiverBasisButton->setDown(false);
  this->hasChanged(false);
}

void RotateDataSettingDialog::setSettings(
    Data::ProjectSettings &projectSettings) {
  //    auto mult = _multEdit->text().toInt();
  //    projectSettings.getTestMultParameters().setMult(mult);
  std::vector<Eigen::Matrix3f> matrixs;
  if (_toEBasisButton->isDown()) {
    Eigen::Matrix3f matrix;
    matrix << 1, 0, 0, 0, 1, 0, 0, 0, 1;
    for (int i = 0; i < 3; ++i) {
      matrixs.push_back(matrix.replicate(3, 3));
    }
  } else if (_toReceiverBasisButton->isDown()) {
    //      matrix << ...
  }
  projectSettings.getRotateDataParameters().setMatrixsBasisTo(matrixs);
}
} // namespace ProjectOperation
