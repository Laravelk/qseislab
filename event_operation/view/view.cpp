#include "view.h"

#include "common_view/graphiccontroller.h"
#include "common_view/infoevent.h"
#include "filemanager.h"
#include "wellmanager.h"

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

// #include <iostream> // TODO: delete

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace Generic {
View::View(const std::unique_ptr<Data::SeismEvent> &event, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint) {

  commonSetting();
  _infoEvent->setEnabled(true);
  _polarizationEventButton->setEnabled(true);
  _infoEvent->update(event);
  _graphicEvent->update(event);
  _okButton->setEnabled(true);
  _okButton->setFocus();

  // Layout`s
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(_infoEvent);
  leftLayout->addStretch(1);

  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(_okButton);
  buttonsLayout->addWidget(_cancelButton);
  buttonsLayout->addWidget(_addWaveButton);
  buttonsLayout->addWidget(_polarizationEventButton);

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

View::View(const std::map<QUuid, QString> &wellNames_map, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _wellManagersLayout(new QVBoxLayout()),
      _addButtonManagers(new QPushButton("Add")),
      _wellNames_map(wellNames_map) {

  commonSetting();

  // Setting`s
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
              int count = _wellManagersLayout->count();
              if (3 > count) {
                _addButtonManagers->setEnabled(true);
                _addButtonManagers->click();
              }
              for (int i = 0; i < count; ++i) {
                QLayoutItem *child = _wellManagersLayout->itemAt(i);
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
    _wellManagersLayout->insertWidget(_wellManagersLayout->count() - 1,
                                      wellManager);
  });
  QHBoxLayout *buttonLayoutManagers = new QHBoxLayout();
  buttonLayoutManagers->addStretch(1);
  buttonLayoutManagers->addWidget(_addButtonManagers);
  _wellManagersLayout->addLayout(buttonLayoutManagers);
  _addButtonManagers->click();
  // Setting`s end

  // Layout`s
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(_infoEvent);
  leftLayout->addLayout(_wellManagersLayout);
  leftLayout->addStretch(1);

  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(_okButton);
  buttonsLayout->addWidget(_cancelButton);
  buttonsLayout->addWidget(_addWaveButton);
  buttonsLayout->addWidget(_polarizationEventButton);

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
  _polarizationEventButton->setEnabled(true);
  _infoEvent->update(event);
  //  _addButtonManagers->setEnabled(true);
  _graphicEvent->update(event);
  _okButton->setEnabled(true);
  _okButton->setFocus();
}

void View::update(const std::unique_ptr<SeismEvent> &event, const QUuid &uuid,
                  const QString &wellName) {
  assert(nullptr != _wellManagersLayout);

  _wellNames_map[uuid] = wellName;
  WellManager *manager = qobject_cast<WellManager *>(
      _wellManagersLayout->itemAt(_wellManagersLayout->count() - 2)->widget());
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

ChartGesture *View::getChartGesture() { return _graphicEvent->getModel(); }

void View::commonSetting() {
  // Setting`s
  _infoEvent = new InfoEvent(this);
  _graphicEvent = new GraphicController(this);
  _okButton = new QPushButton("Ok", this);
  _cancelButton = new QPushButton("Cancel", this);
  _addWaveButton = new QPushButton("+", this);
  _polarizationEventButton = new QPushButton("Polarization Analysis", this);

  setWindowTitle("SeismWindow");
  setMinimumSize(1100, 590);

  _infoEvent->setDisabled(true);
  _polarizationEventButton->setDisabled(true);

  QMenu *addWaveButtonMenu = new QMenu(_addWaveButton);
  _addPWave = new QAction("PWAVE", _addWaveButton);
  _addSWave = new QAction("SWAVE", _addWaveButton);
  addWaveButtonMenu->addAction(_addPWave);
  addWaveButtonMenu->addAction(_addSWave);
  _addWaveButton->setMenu(addWaveButtonMenu);

  _okButton->setDisabled(true);
  // Setting`s end

  // Connecting
  connect(_graphicEvent, &EventOperation::GraphicController::sendPicksInfo,
          [this](auto type, auto num, auto l_val, auto pick_val, auto r_val) {
            emit sendPicksInfo(type, num, l_val, pick_val, r_val);
          });
  connect(_polarizationEventButton, &QPushButton::clicked,
          [this]() { emit createPolarizationAnalysisWindow(); });
  connect(_okButton, &QPushButton::clicked, this, &View::accept);
  connect(_cancelButton, &QPushButton::clicked, this, &View::reject);
  connect(_addPWave, &QAction::triggered, [this]() {
    _graphicEvent->getView()->setWaveAddTriggerFlag(Data::SeismWavePick::PWAVE);
  });
  connect(_addSWave, &QAction::triggered, [this]() {
    _graphicEvent->getView()->setWaveAddTriggerFlag(Data::SeismWavePick::SWAVE);
  });
  // Connecting end
}

} // namespace Generic
} // namespace EventOperation
