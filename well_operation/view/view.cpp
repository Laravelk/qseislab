#include "view.h"

#include "data/seismwell.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismWell SeismWell;

namespace WellOperation {
View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _wellsTable(new TableAssistant(TableAssistant::ForWells, this)),
      _saveButton(new QPushButton("Save")) {

  // Setting`s
  setWindowTitle("Wells");
  setMinimumWidth(440);

  _saveButton->setDisabled(true);
  QPushButton *addWellButton = new QPushButton("Add Well");
  QPushButton *cancelButton = new QPushButton("Cancel");
  // Setting`s end

  // Connecting
  connect(_wellsTable, &TableAssistant::removeClicked,
          [this](auto &uuid) { emit removeWellClicked(uuid); });
  connect(addWellButton, &QPushButton::clicked, this,
          &View::handleAddWellClicked);
  connect(_saveButton, &QPushButton::clicked, this, &View::accept);
  connect(cancelButton, &QPushButton::clicked, this, &View::reject);
  // Connecting end

  // Layout`s
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(addWellButton);
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(_saveButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_wellsTable);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void View::addWell(const std::shared_ptr<SeismWell> &well) {
  _wellsTable->add<SeismWell>(well);
}

void View::updateWell(const std::shared_ptr<SeismWell> &well) {
  _addWellManager->update(well);
  _wellsTable->update<SeismWell>(well);
}

void View::removeWell(const QUuid &uuid) { _wellsTable->remove(uuid); }

void View::settingWellInfo(const std::shared_ptr<Data::SeismWell> &well) {
  _addWellManager->settingWellInfo(well);
}

void View::setNotification(const QString &text) {
  QMessageBox *msg = new QMessageBox(QMessageBox::Critical, "Message", text,
                                     QMessageBox::Ok, this);
  msg->show();
}

void View::changed(bool b) {
  _saveButton->setEnabled(b);
  _saveButton->setFocus();
}

void View::handleAddWellClicked() {
  _addWellManager = std::make_unique<AddWellManager>(this);

  connect(_addWellManager.get(), &AddWellManager::sendFilePath,
          [this](auto &path) { emit sendFilePath(path); });
  connect(_addWellManager.get(), &AddWellManager::notify, this,
          &View::setNotification);
  connect(_addWellManager.get(), &AddWellManager::finished, [this](int res) {
    if (QDialog::Accepted == res) {
      emit addWellClicked();
    }
  });

  _addWellManager->setModal(true);
  _addWellManager->show();
}

} // namespace WellOperation
