
#include "rotatedatasettingdialog.h"

#include "data/projectsettings.h"

#include <Eigen/Dense>

#include <QFrame>
#include <QDoubleValidator>
#include <QLabel>

#include <iostream> // TODO: remove


namespace ProjectOperation {
RotateDataSettingDialog::RotateDataSettingDialog(QWidget *parent)
    : SettingDialog(parent), _toEBasisButton(new QRadioButton("To E-basis")),
      _toReceiverBasisButton(new QRadioButton("To Receiver-basis")),
    _toIndefiniteBasisButton(new QRadioButton("To Indefinite-basis")),
    _indefiniteEditWidget(new IndefiniteBasisEditWidget()){

  // Setting`s
  setWindowTitle("Rotate Data Settings");
  _indefiniteEditWidget->setDisabled(true);
  // Setting`s end

  // Connecting
  connect(_toEBasisButton, &QRadioButton::toggled,
          [this] { this->hasChanged(true);
  });
  connect(_toReceiverBasisButton, &QRadioButton::toggled,
          [this] { this->hasChanged(true);
  });
  connect(_toIndefiniteBasisButton, &QRadioButton::toggled,
          [this](bool checked) { this->hasChanged(true);
            _indefiniteEditWidget->setEnabled(checked);
  });
  connect(_indefiniteEditWidget, &IndefiniteBasisEditWidget::changed, [this]{
     this->hasChanged(true);
  });
  // Connecting end

  // Layout`s
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_toEBasisButton);
  mainLayout->addWidget(_toReceiverBasisButton);
  mainLayout->addWidget(_toIndefiniteBasisButton);
  mainLayout->addWidget(_indefiniteEditWidget);
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
      _indefiniteEditWidget->setBasis(projectSettings->getRotateDataParameters().getOrientation());
      _toIndefiniteBasisButton->setChecked(true);
    break;
  }

  this->hasChanged(false);
}

void RotateDataSettingDialog::setSettings(
    Data::ProjectSettings *const projectSettings) {

    auto& rotateSetting = projectSettings->getRotateDataParameters();

  if (_toEBasisButton->isChecked()) {
    rotateSetting.setMode(
        RotateData::Parameters::Mode::EBASIS);
  }
  if (_toReceiverBasisButton->isChecked()) {
    rotateSetting.setMode(
        RotateData::Parameters::Mode::RECEIVERS);
  }
  if(_toIndefiniteBasisButton->isChecked())
  {
      rotateSetting.setMode(
          RotateData::Parameters::Mode::INDEFINITE);
      rotateSetting.setOrientation(_indefiniteEditWidget->getBasis());
  }
}

IndefiniteBasisEditWidget::IndefiniteBasisEditWidget(QWidget *parent) : QFrame(parent) {
    auto validator = new QDoubleValidator(-10.0, 10.0, 3);
    for(int i = 0; i < 9; ++i) {
        crds[i] = new QLineEdit("0,0");
        crds[i]->setValidator(validator);
        connect(crds[i], &QLineEdit::textChanged, [this]{
            emit changed();
        });
    }

    auto mainLayout = new QVBoxLayout();
    for(int i = 0; i < 3; ++i) {
        auto rowLayout = new QHBoxLayout();
        rowLayout->addWidget(new QLabel(" [ "));
        rowLayout->addWidget(crds[3*i]);
        rowLayout->addWidget(new QLabel(" , "));
        rowLayout->addWidget(crds[3*i + 1]);
        rowLayout->addWidget(new QLabel(" , "));
        rowLayout->addWidget(crds[3*i + 2]);
        rowLayout->addWidget(new QLabel(" ] "));
        mainLayout->addLayout(rowLayout);
    }
    setLayout(mainLayout);
}

void IndefiniteBasisEditWidget::setBasis(const Eigen::Matrix3f &matrix)
{
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            crds[3*i + j]->setText(QString::number(static_cast<double>(matrix(i,j))));
        }
    }
}

Eigen::Matrix3f IndefiniteBasisEditWidget::getBasis() const
{
    Eigen::Matrix3f matrix;
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            matrix(i,j) = static_cast<float>(crds[3*i + j]->text().toDouble());
        }
    }
    return matrix;
}

bool IndefiniteBasisEditWidget::isValid() const{
    // TODO: implement
    return false;
}

} // namespace ProjectOperation
