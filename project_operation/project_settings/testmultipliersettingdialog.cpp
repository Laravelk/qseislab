#include "testmultipliersettingdialog.h"

#include "data/projectsettings.h"

#include <QIntValidator>

namespace ProjectOperation {
TestMultiplierSettingDialog::TestMultiplierSettingDialog(QWidget *parent)
    : SettingDialog(parent), _multEdit(new QLineEdit()) {
  // Setting`s
  setWindowTitle("Test Mult Settings");

  _multEdit->setValidator(new QIntValidator(1, 10));
  // Setting`s end

  // Connecting
  connect(_multEdit, &QLineEdit::textChanged,
          [this] { this->hasChanged(true); });
  // Connecting end

  // Layout`s
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_multEdit);
  mainLayout->addStretch(1);
  mainLayout->addLayout(_buttonsLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void TestMultiplierSettingDialog::update(
    const Data::ProjectSettings &projectSettings) {
  _multEdit->setText(
      QString::number(projectSettings.getTestMultParameters().getMult()));
  this->hasChanged(false);
}

void TestMultiplierSettingDialog::setSettings(
    Data::ProjectSettings &projectSettings) {
  auto mult = _multEdit->text().toInt();
  projectSettings.getTestMultParameters().setMult(mult);
}

} // namespace ProjectOperation
