#include "ffilteringdatasettingdialog.h"

#include "data/projectsettings.h"

#include <QFormLayout>
#include <QIntValidator>
#include <QLabel>

namespace ProjectOperation {
FFilteringDataSettingDialog::FFilteringDataSettingDialog(QWidget *parent)
    : SettingDialog(parent), _f1Edit(new QLineEdit()), _f2Edit(new QLineEdit()),
      _f3Edit(new QLineEdit()), _f4Edit(new QLineEdit()) {
  // Setting`s
  setWindowTitle("FFiltering Settings");

  auto validator = new QIntValidator(1, 1500);
  _f1Edit->setValidator(validator);
  _f1Edit->setValidator(validator);
  _f1Edit->setValidator(validator);
  _f1Edit->setValidator(validator);
  // Setting`s end

  // Connecting
  connect(_f1Edit, &QLineEdit::textChanged, this, &FFilteringDataSettingDialog::inputChange);
  connect(_f2Edit, &QLineEdit::textChanged, this, &FFilteringDataSettingDialog::inputChange);
  connect(_f3Edit, &QLineEdit::textChanged, this, &FFilteringDataSettingDialog::inputChange);
  connect(_f4Edit, &QLineEdit::textChanged, this, &FFilteringDataSettingDialog::inputChange);
  // Connecting end

  // Layout`s
  QFormLayout *formLayout = new QFormLayout();
  auto row = new QHBoxLayout();
  row->addWidget(_f1Edit);
  row->addWidget(new QLabel("Hz"));
  formLayout->addRow("F1", row);

  row = new QHBoxLayout();
  row->addWidget(_f2Edit);
  row->addWidget(new QLabel("Hz"));
  formLayout->addRow("F2", row);

  row = new QHBoxLayout();
  row->addWidget(_f3Edit);
  row->addWidget(new QLabel("Hz"));
  formLayout->addRow("F3", row);

  row = new QHBoxLayout();
  row->addWidget(_f4Edit);
  row->addWidget(new QLabel("Hz"));
  formLayout->addRow("F4", row);

  QHBoxLayout *hLayout = new QHBoxLayout();
  hLayout->addStretch(1);
  hLayout->addLayout(formLayout);
  hLayout->addStretch(1);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addLayout(hLayout);
  mainLayout->addStretch(1);
  mainLayout->addLayout(_buttonsLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void FFilteringDataSettingDialog::update(
    Data::ProjectSettings const *const projectSettings) {
  _f1Edit->setText(
      QString::number(projectSettings->getFFilteringParameters().getF1()));
  _f2Edit->setText(
      QString::number(projectSettings->getFFilteringParameters().getF2()));
  _f3Edit->setText(
      QString::number(projectSettings->getFFilteringParameters().getF3()));
  _f4Edit->setText(
      QString::number(projectSettings->getFFilteringParameters().getF4()));
  this->hasChanged(false);
}

void FFilteringDataSettingDialog::setSettings(
    Data::ProjectSettings *const projectSettings) {
  auto& fFilteringParameters = projectSettings->getFFilteringParameters();
  fFilteringParameters.setF1(_f1Edit->text().toInt());
  fFilteringParameters.setF2(_f2Edit->text().toInt());
  fFilteringParameters.setF3(_f3Edit->text().toInt());
  fFilteringParameters.setF4(_f4Edit->text().toInt());
}

void FFilteringDataSettingDialog::inputChange() {
    this->hasChanged(true);

    // check correct input
    auto f1_str = _f1Edit->text();
    auto f2_str = _f2Edit->text();
    auto f3_str = _f3Edit->text();
    auto f4_str = _f4Edit->text();

    int f1 = f1_str.toInt();
    int f2 = f2_str.toInt();
    int f3 = f3_str.toInt();
    int f4 = f4_str.toInt();

    if(f1_str.isEmpty() || f2_str.isEmpty() || f3_str.isEmpty() || f4_str.isEmpty()) {
         this->setCorrect(QString("Имеются пустые поля"));
    } else if (0 > f1 || f1 > f2 || f2 > f3 || f3 > f4) {
       this->setCorrect(QString("Некорректные частоты"));
    } else {
        this->setCorrect(true);
    }
    // check correct input end
}
} // namespace ProjectOperation