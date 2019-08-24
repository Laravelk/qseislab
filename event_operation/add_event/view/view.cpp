#include "view.h"

#include "event_operation/share_view/graphicevent.h"
#include "event_operation/share_view/infoevent.h"
#include "filemanager.h"

#include <QBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace AddEvent {
View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _fileManager(new FileManager(this)), _infoEvent(new InfoEvent(this)),
      _graphicEvent(new GraphicEvent(this)),
      _addButton(new QPushButton("Add", this)),
      _cancelButton(new QPushButton("Cancel", this)) {

  setWindowTitle("SeismWindow");
  setMinimumSize(1100, 590);

  connect(_fileManager, SIGNAL(sendFilePath(const QString &)), this,
          SLOT(recvFilePath(const QString &)));

  _infoEvent->setDisabled(true);

  _addButton->setDisabled(true);
  connect(_addButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(_fileManager);
  leftLayout->addWidget(_infoEvent);
  leftLayout->addStretch(1);

  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(_addButton);
  buttonsLayout->addWidget(_cancelButton);

  QVBoxLayout *graphicLayout = new QVBoxLayout();
  graphicLayout->addWidget(_graphicEvent->getView(), 10);
  graphicLayout->addStretch(1);
  graphicLayout->addLayout(buttonsLayout);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  //    mainLayout->addStretch(1);
  mainLayout->addLayout(graphicLayout, 10);

  setLayout(mainLayout);
}

void View::update(const std::unique_ptr<Data::SeismEvent> &event) {
  if (!event) {
    _fileManager->clear();
    _graphicEvent->clear();
    _infoEvent->clear();
    _infoEvent->setDisabled(true);
    _addButton->setDisabled(true);
    return;
  }
  _infoEvent->setDisabled(false);
  _infoEvent->update(event);
  _graphicEvent->update(event);
  _addButton->setDisabled(false);
  _addButton->setFocus();
}

void View::setNotification(const QString &text) {
  QMessageBox *msg = new QMessageBox(QMessageBox::Critical, "Message", text,
                                     QMessageBox::Ok, this);
  msg->show();
}

void View::settingEventInfo(
    const std::unique_ptr<Data::SeismEvent> &event) const {
  _infoEvent->settingEventInfo(event);
}

void View::recvFilePath(const QString &path) { emit sendFilePath(path); }

} // namespace AddEvent
} // namespace EventOperation
