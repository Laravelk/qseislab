#include "addwellmanager.h"

#include "data/seismwell.h"
#include "filemanager.h"

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace WellOperation {
AddWellManager::AddWellManager(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _fileManager(new FileManager()), _nameLineEdit(new QLineEdit()),
      _pointAmountLabel(new QLabel()), _addButton(new QPushButton("Add")) {

  // Setting`s
  setWindowTitle("Add Well");

  _nameLineEdit->setDisabled(true);

  _addButton->setDisabled(true);

  QPushButton *cancelButton = new QPushButton("Cancel");
  // Setting`s end

  // Connecting
  connect(_fileManager, &FileManager::sendFilePath,
          [this](auto &path) { emit sendFilePath(path); });

  connect(_addButton, &QPushButton::clicked, this, &AddWellManager::accept);

  connect(cancelButton, &QPushButton::clicked, this, &AddWellManager::reject);
  // Connecting end

  // Layout`s
  QFormLayout *infoWellLayout = new QFormLayout();
  infoWellLayout->addRow("Name: ", _nameLineEdit);
  infoWellLayout->addRow("Point Amount: ", _pointAmountLabel);

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(_addButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_fileManager);
  mainLayout->addLayout(infoWellLayout);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void AddWellManager::update(Data::SeismWell const *const well) {
  if (!well) {
    _fileManager->clear();
    _nameLineEdit->clear();
    _pointAmountLabel->clear();
    _addButton->setDisabled(true);
    _nameLineEdit->setDisabled(true);
    return;
  }

  _nameLineEdit->setText(well->getName());
  _pointAmountLabel->setNum(well->getPointsAmount());
  _addButton->setDisabled(false);
  _nameLineEdit->setDisabled(false);
}

void AddWellManager::settingWellInfo(Data::SeismWell *const well) {
  well->setName(_nameLineEdit->text());
}

} // namespace WellOperation
