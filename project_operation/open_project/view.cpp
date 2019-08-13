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
    : QDialog(parent), _fileManager(new FileManager(this)),
      _infoProject(new InfoProject(InfoProject::CLEAN, this)),
      _openButton(new QPushButton("Open", this)),
      _cancelButton(new QPushButton("Cancel", this)) {
  setWindowTitle("Open Project");

  connect(_fileManager, SIGNAL(sendFilePath(const QString &)), this,
          SLOT(recvFilePath(const QString &)));

  _openButton->setDisabled(true);

  connect(_openButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

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
}

void View::update(const std::unique_ptr<Data::SeismProject> &project) {
  if (!project) {
    _fileManager->clear();
    _infoProject->clear();
    _openButton->setDisabled(true);
    return;
  }
  _infoProject->setName(project->getName());
  _infoProject->setDate(project->getDateTime().date());
  _infoProject->setTime(project->getDateTime().time());
  _openButton->setDisabled(false);
}

void View::setNotification(const QString &text) {
  QMessageBox *msg = new QMessageBox(this);
  msg->setWindowTitle("Message");
  msg->addButton(QMessageBox::StandardButton::Ok);
  msg->setText(text);
  msg->exec();
}

void View::setName(const QString &name) { _infoProject->setName(name); }

QString View::getName() const { return _infoProject->getName(); }

void View::setDate(const QDate &date) { _infoProject->setDate(date); }

QDate View::getDate() const { return _infoProject->getDate(); }

void View::setTime(const QTime &time) { _infoProject->setTime(time); }

QTime View::getTime() const { return _infoProject->getTime(); }

void View::recvFilePath(const QString &path) { emit sendFilePath(path); }

} // namespace OpenProject
} // namespace ProjectOperation
