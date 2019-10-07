#include "view.h"

#include "data/seismproject.h"
#include "project_operation/share_view/filemanager.h"
#include "project_operation/share_view/infoproject.h"

#include <QBoxLayout>
#include <QMessageBox>

typedef Data::SeismProject SeismProject;

namespace ProjectOperation {
namespace OpenProject {
View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _fileManager(new FileManager(this)),
      _infoProject(new InfoProject(InfoProject::CLEAN, this)),
      _openButton(new QPushButton("Open", this)),
      _cancelButton(new QPushButton("Cancel", this)) {

  // Setting`s
  setWindowTitle("Open Project");

  _infoProject->setDisabled(true);
  _openButton->setDisabled(true);
  // Setting`s end

  // Connecting
  connect(_fileManager, &FileManager::sendFilePath,
          [this](auto &path) { emit sendFilePath(path); });

  connect(_openButton, &QPushButton::clicked, this, &View::accept);
  connect(_cancelButton, &QPushButton::clicked, this, &View::reject);
  // Connecting end

  // Layout`s
  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(_openButton);
  buttonsLayout->addWidget(_cancelButton);

  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->addWidget(_fileManager);
  vLayout->addWidget(_infoProject);
  vLayout->addStretch(1);
  vLayout->addLayout(buttonsLayout);

  setLayout(vLayout);
  // Layout`s end
}

void View::update(const std::unique_ptr<Data::SeismProject> &project) {
  if (!project) {
    _fileManager->clear();
    _infoProject->clear();
    _infoProject->setDisabled(true);
    _openButton->setDisabled(true);
    return;
  }
  _infoProject->update(project);
  _openButton->setDisabled(false);
}

void View::setNotification(const QString &text) {
  QMessageBox *msg = new QMessageBox(QMessageBox::Critical, "Message", text,
                                     QMessageBox::Ok, this);
  msg->show();
}

void View::settingProjectInfo(
    const std::unique_ptr<Data::SeismProject> &project) {
  _infoProject->settingProjectInfo(project);
}

} // namespace OpenProject
} // namespace ProjectOperation
