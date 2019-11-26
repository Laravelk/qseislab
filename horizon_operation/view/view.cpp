#include "view.h"

#include "addhorizonmanager.h"
#include "data/seismhorizon.h"
#include "share_view/tableassistant.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismHorizon SeismHorizon;

namespace HorizonOperation {
View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _horizonsTable(new TableAssistant(TableAssistant::ForHorizons, this)),
      _saveButton(new QPushButton("Save")) {

  // Setting`s
  setWindowTitle("Horizons");
  setMinimumWidth(440);

  _saveButton->setDisabled(true);
  QPushButton *addHorizonButton = new QPushButton("Add Horizon");
  QPushButton *cancelButton = new QPushButton("Cancel");
  // Setting`s end

  // Connecting
  connect(_horizonsTable, &TableAssistant::removeClicked,
          [this](auto &uuid) { emit removeHorizonClicked(uuid); });
  connect(_saveButton, &QPushButton::clicked, this, &View::accept);
  connect(addHorizonButton, &QPushButton::clicked, this,
          &View::handleAddHorizonClicked);
  connect(cancelButton, &QPushButton::clicked, this, &View::reject);
  // Connecting end

  // Layout`s
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(addHorizonButton);
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(_saveButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_horizonsTable);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void View::addHorizon(SeismHorizon const *const horizon) {
  _horizonsTable->add<SeismHorizon>(horizon);
  isChanged(true);
}

void View::updateHorizon(SeismHorizon const *const horizon) {
  _addHorizonManager->update(horizon);
  isChanged(true);
}

void View::removeHorizon(const QUuid &uuid) {
  _horizonsTable->remove(uuid);
  isChanged(true);
}

void View::settingHorizonInfo(SeismHorizon *const horizon) {
  _addHorizonManager->settingHorizonInfo(horizon);
}

void View::setNotification(const QString &text) {
  QMessageBox *msg = new QMessageBox(QMessageBox::Critical, "Message", text,
                                     QMessageBox::Ok, this);
  msg->show();
}

void View::handleAddHorizonClicked() {
  _addHorizonManager = std::make_unique<AddHorizonManager>(this);

  connect(_addHorizonManager.get(), &AddHorizonManager::sendFilePath,
          [this](auto &path) { emit sendFilePath(path); });
  connect(_addHorizonManager.get(), &AddHorizonManager::notify, this,
          &View::setNotification);
  connect(_addHorizonManager.get(), &AddHorizonManager::finished,
          [this](int res) {
            if (QDialog::Accepted == res) {
              emit addHorizonClicked();
            }
          });

  _addHorizonManager->setModal(true);
  _addHorizonManager->show();
}

void View::isChanged(bool b) {
  _saveButton->setEnabled(b);
  if (b)
    _saveButton->setFocus();
}
} // namespace HorizonOperation
