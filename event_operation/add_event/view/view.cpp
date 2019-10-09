#include "view.h"

#include "event_operation/share_view/controller.h"
#include "event_operation/share_view/infoevent.h"
#include "filemanager.h"
#include "wellmanager.h"

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace AddEvent {
View::View(const std::map<QUuid, QString> &wellNames_map, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _infoEvent(new InfoEvent(this)), _wellManegersLayout(new QVBoxLayout()),
      _addButtonManagers(new QPushButton("Add")),
      _graphicEvent(new Controller(this)),
      _okButton(new QPushButton("Ok", this)),
      _cancelButton(new QPushButton("Cancel", this)),
      _addWaveButton(new QPushButton("+", this)),
      _wellNames_map(wellNames_map) {

  // Setting`s
  setWindowTitle("SeismWindow");
  setMinimumSize(1100, 590);

  _infoEvent->setDisabled(true);

  connect(_addButtonManagers, &QPushButton::clicked, [this]() {
    _addButtonManagers->setDisabled(true);
    auto wellManager = new WellManager(_wellNames_map);
    connect(wellManager, &WellManager::sendWellUuidAndFilePath,
            [this](auto &uuid_path) {
              _addButtonManagers->setEnabled(true);
              emit sendWellUuidAndFilePath(uuid_path);
            });
    connect(wellManager, &WellManager::removeClicked,
            [this](QWidget *sender, auto uuid) {
              if (nullptr == sender) {
                return;
              }
              int count = _wellManegersLayout->count();
              if (3 > count) {
                _addButtonManagers->setEnabled(true);
                _addButtonManagers->click();
              }
              for (int i = 0; i < count; ++i) {
                QLayoutItem *child = _wellManegersLayout->itemAt(i);
                if (sender == child->widget()) {
                  delete child->widget();
                  if (count - 2 == i && 2 != count) {
                    _addButtonManagers->setEnabled(true);
                  }
                  break;
                }
              }
              if (!uuid.isNull()) {
                emit sendWellUuidForRemove(uuid);
              }
            });
    _wellManegersLayout->insertWidget(_wellManegersLayout->count() - 1,
                                      wellManager);
  });
  QHBoxLayout *buttonLayoutManagers = new QHBoxLayout();
  buttonLayoutManagers->addStretch(1);
  buttonLayoutManagers->addWidget(_addButtonManagers);
  _wellManegersLayout->addLayout(buttonLayoutManagers);
  _addButtonManagers->click();

  _okButton->setDisabled(true);
  // Setting`s end

  // Connecting
  connect(_graphicEvent, &EventOperation::Controller::sendTypeNumCompY,
          [this](auto type, auto num, auto val) {
            emit sendWavePickTypeNumCompY(type, num, val);
          });
  connect(_addWaveButton, &QPushButton::clicked,
          [this]() { _graphicEvent->addPick(); });
  connect(_okButton, &QPushButton::clicked, this, &View::accept);
  connect(_cancelButton, &QPushButton::clicked, this, &View::reject);
  // Connecting end

  // Layout`s
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(_infoEvent);
  leftLayout->addLayout(_wellManegersLayout);
  leftLayout->addStretch(1);

  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(_okButton);
  buttonsLayout->addWidget(_cancelButton);
  buttonsLayout->addWidget(_addWaveButton);

  QVBoxLayout *graphicLayout = new QVBoxLayout();
  graphicLayout->addWidget(_graphicEvent->getView(), 10);
  graphicLayout->addStretch(1);
  graphicLayout->addLayout(buttonsLayout);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  //    mainLayout->addStretch(1);
  mainLayout->addLayout(graphicLayout, 10);

  setLayout(mainLayout);
  // Layout`s end
}

void View::update(const std::unique_ptr<SeismEvent> &event,
                  const QUuid &removedWellUuid) {
  _wellNames_map.erase(removedWellUuid);

  _infoEvent->setEnabled(true);
  _infoEvent->update(event);
  //  _addButtonManagers->setEnabled(true);
  _graphicEvent->update(event);
  _okButton->setEnabled(true);
  _okButton->setFocus();
}

void View::update(const std::unique_ptr<SeismEvent> &event, const QUuid &uuid,
                  const QString &wellName) {
  _wellNames_map[uuid] = wellName;
  WellManager *manager = qobject_cast<WellManager *>(
      _wellManegersLayout->itemAt(_wellManegersLayout->count() - 2)->widget());
  if (manager) {
    manager->updateWellNames(_wellNames_map);
  } else {
    qDebug()
        << "qobject_cast<WellManager*> == nullptr (for updating wellNames_map)";
  }

  _infoEvent->setEnabled(true);
  _infoEvent->update(event);
  //  _addButtonManagers->setEnabled(true);
  _graphicEvent->update(event);
  _okButton->setEnabled(true);
  _okButton->setFocus();
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

} // namespace AddEvent
} // namespace EventOperation
