#include "addhorizonmanager.h"

#include "data/seismhorizon.h"
#include "filemanager.h"

#include <QFormLayout>
#include <QPushButton>

namespace HorizonOperation {
AddHorizonManager::AddHorizonManager(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _fileManager(new FileManager()), _nameLineEdit(new QLineEdit()),
      _NxLineEdit(new QLineEdit()), _NyLineEdit(new QLineEdit()),
      _pointAmountLabel(new QLabel()), _addButton(new QPushButton("Add")) {

  // Setting`s
  setWindowTitle("Add Horizon");

  _nameLineEdit->setDisabled(true);
  _NxLineEdit->setDisabled(true);
  _NyLineEdit->setDisabled(true);

  _addButton->setDisabled(true);

  QPushButton *cancelButton = new QPushButton("Cancel");
  // Setting`s end

  // Connecting
  connect(_fileManager, &FileManager::sendFilePath,
          [this](auto &path) { emit sendFilePath(path); });

  connect(_addButton, &QPushButton::clicked, [this] {
    const int pointAmount = _pointAmountLabel->text().toInt();
    const int Nx = _NxLineEdit->text().toInt();
    const int Ny = _NyLineEdit->text().toInt();
    if (pointAmount != Nx * Ny) {
      emit notify("Custom Nx * Ny != real size of horizon");
    } else {
      accept();
    }
  });

  connect(cancelButton, &QPushButton::clicked, this,
          &AddHorizonManager::reject);
  // Connecting end

  // Layout`s
  QFormLayout *infoHorizonLayout = new QFormLayout();
  infoHorizonLayout->addRow("Name: ", _nameLineEdit);
  infoHorizonLayout->addRow("Nx: ", _NxLineEdit);
  infoHorizonLayout->addRow("Ny: ", _NyLineEdit);
  infoHorizonLayout->addRow("Point Amount: ", _pointAmountLabel);

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(_addButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_fileManager);
  mainLayout->addLayout(infoHorizonLayout);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void AddHorizonManager::update(
    const std::unique_ptr<Data::SeismHorizon> &horizon) {
  if (!horizon) {
    _fileManager->clear();
    _nameLineEdit->clear();
    _NxLineEdit->clear();
    _NyLineEdit->clear();
    _pointAmountLabel->clear();
    _addButton->setDisabled(true);
    _nameLineEdit->setDisabled(true);
    _NxLineEdit->setDisabled(true);
    _NyLineEdit->setDisabled(true);
    return;
  }
  _nameLineEdit->setText(horizon->getName());
  _NxLineEdit->setText(QString::number(horizon->getNx()));
  _NyLineEdit->setText(QString::number(horizon->getNy()));
  _pointAmountLabel->setNum(horizon->getPointsAmount());
  _addButton->setDisabled(false);
  _nameLineEdit->setDisabled(false);
  _NxLineEdit->setDisabled(false);
  _NyLineEdit->setDisabled(false);
}

void AddHorizonManager::settingHorizonInfo(
    const std::unique_ptr<Data::SeismHorizon> &horizon) {
  horizon->setName(_nameLineEdit->text());
  horizon->setNx(_NxLineEdit->text().toInt());
  horizon->setNy(_NyLineEdit->text().toInt());
}

} // namespace HorizonOperation
