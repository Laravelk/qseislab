#include "addhorizonmanager.h"

#include "data/seismhorizon.h"
#include "filemanager.h"

#include <QFormLayout>
#include <QPushButton>

namespace HorizonOperation {
AddHorizonManager::AddHorizonManager(QWidget *parent)
    : QDialog(parent), _fileManager(new FileManager()),
      _nameLineEdit(new QLineEdit()), _pointNumberLabel(new QLabel()),
      _addButton(new QPushButton("Add")) {

  setWindowTitle("Add Horizon");

  connect(_fileManager, SIGNAL(sendFilePath(const QString &)), this,
          SLOT(recvFilePath(const QString &)));

  _addButton->setDisabled(true);
  connect(_addButton, SIGNAL(clicked()), this, SLOT(accept()));

  QPushButton *cancelButton = new QPushButton("Cancel");
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

  QFormLayout *infoHorizonLayout = new QFormLayout();
  infoHorizonLayout->addRow("Name: ", _nameLineEdit);
  infoHorizonLayout->addRow("Point Number: ", _pointNumberLabel);

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
}

void AddHorizonManager::update(
    const std::unique_ptr<Data::SeismHorizon> &horizon) {
  if (!horizon) {
    _fileManager->clear();
    _nameLineEdit->clear();
    _pointNumberLabel->clear();
    _addButton->setDisabled(true);
    return;
  }
  _nameLineEdit->setText(horizon->getName());
  _pointNumberLabel->setNum(horizon->getPointsNumber());
  _addButton->setDisabled(false);
}

void AddHorizonManager::recvFilePath(const QString &filePath) {
  emit sendFilePath(filePath);
}

} // namespace HorizonOperation
