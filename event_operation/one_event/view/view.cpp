#include "view.h"

#include "event_operation/share/view/graphiccontroller.h"
#include "event_operation/share/view/infoevent.h"
#include "filemanager.h"
#include "wellmanager.h"

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

#include <assert.h>

//#include "data/seismevent.h" // TODO: delete
// #include <iostream> // TODO: delete

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace OneEvent {
View::View(const std::set<QString> &eventNames,
           const std::unique_ptr<Data::SeismEvent> &event, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _eventNames(eventNames) {

  commonSetting();
  _infoEvent->setEnabled(true);
  //  _polarizationEventButton->setEnabled(true);
  _infoEvent->update(event);
  _graphicEvent->update(event);
  _okButton->setEnabled(true);
  _okButton->setFocus();

  _infoEvent->update(event);

  // Layout`s
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(_infoEvent);
  leftLayout->addStretch(1);

  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(_okButton);
  buttonsLayout->addWidget(_cancelButton);
  //  buttonsLayout->addWidget(_addWaveButton);
  //  buttonsLayout->addWidget(_polarizationEventButton);

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

View::View(const std::set<QString> &eventNames,
           const std::map<QUuid, QString> &wellNames_map, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _wellManagersLayout(new QVBoxLayout()),
      _addButtonManagers(new QPushButton("Add")), _wellNames_map(wellNames_map),
      _eventNames(eventNames) {

  commonSetting();

  // Setting`s
  connect(_addButtonManagers, &QPushButton::clicked, [this]() {
    _addButtonManagers->setDisabled(true);
    auto wellManager = new WellManager(_wellNames_map);
    connect(wellManager, &WellManager::sendWellUuidAndFilePath,
            [this](auto &uuid, auto &path) {
              _addButtonManagers->setEnabled(true);
              emit sendWellUuidAndFilePath(uuid, path);
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

  //  QVBoxLayout *editGraphicMenuLayout = new QVBoxLayout();
  //  editGraphicMenuLayout->addStretch(1);
  //  editGraphicMenuLayout->addWidget(_noneWiggle);
  //  editGraphicMenuLayout->addWidget(_positiveWiggle);
  //  editGraphicMenuLayout->addWidget(_negativeWiggle);
  //  editGraphicMenuLayout->addWidget(_hideComponentsTable);
  //  editGraphicMenuLayout->addWidget(_clippingSlider);
  //  editGraphicMenuLayout->addWidget(_clippintSliderLabel);
  //  editGraphicMenuLayout->addWidget(_gainSlider);
  //  editGraphicMenuLayout->addWidget(_gainSliderLabel);
  //  editGraphicMenuLayout->addStretch(1);
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
  //  buttonsLayout->addWidget(_addWaveButton);
  //  buttonsLayout->addWidget(_polarizationEventButton);

  QVBoxLayout *graphicLayout = new QVBoxLayout();
  graphicLayout->addWidget(_graphicEvent->getView(), 10);
  graphicLayout->addStretch(1);
  graphicLayout->addLayout(buttonsLayout);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  //    mainLayout->addStretch(1);
  mainLayout->addLayout(graphicLayout, 10);
  //  mainLayout->addLayout(editGraphicMenuLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void View::update(const std::unique_ptr<SeismEvent> &event,
                  const QUuid &removedWellUuid) {
  //  std::cout << "update event-name: " << event->getName().toStdString()
  //            << std::endl; // TODO: remove

  _wellNames_map.erase(removedWellUuid);

  _infoEvent->setEnabled(true);
  //  _polarizationEventButton->setEnabled(true);
  _infoEvent->update(event);
  //  _addButtonManagers->setEnabled(true);
  _graphicEvent->update(event);
  //  showGraphicMenu();
  _okButton->setEnabled(true);
  _okButton->setFocus();
}

void View::update(const std::unique_ptr<SeismEvent> &event, const QUuid &uuid,
                  const QString &wellName) {
  assert(nullptr != _wellManagersLayout);

  //  std::cout << "update event-name: " << event->getName().toStdString()
  //            << std::endl; // TODO: remove

  _wellNames_map[uuid] = wellName;
  WellManager *manager = qobject_cast<WellManager *>(
      _wellManagersLayout->itemAt(_wellManagersLayout->count() - 2)->widget());
  if (manager) {
    manager->updateWellNames(_wellNames_map);
  } else {
    qDebug()
        << "qobject_cast<WellManager*> == nullptr (for updating wellNames_map)";
  }

  if (0 == event->getComponentAmount()) {
    _infoEvent->clear();
    _infoEvent->setDisabled(true);

    _graphicEvent->clear();

    _okButton->setDisabled(true);
  } else {
    _infoEvent->setEnabled(true);
    _infoEvent->update(event);
    //  _addButtonManagers->setEnabled(true);
    _graphicEvent->update(event);
    _okButton->setEnabled(true);
    _okButton->setFocus();
  }
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
  //  _addWaveButton = new QPushButton("+", this);
  //  _polarizationEventButton = new QPushButton("Polarization Analysis", this);

  //  settingGraphicMenu();
  //  settingWiggleButton();

  setWindowTitle("SeismWindow");
  setMinimumSize(1300, 590);

  _infoEvent->setDisabled(true);
  //  _polarizationEventButton->setDisabled(true);

  //  QMenu *addWaveButtonMenu = new QMenu(_addWaveButton);
  //  _addPWave = new QAction("PWAVE", _addWaveButton);
  //  _addSWave = new QAction("SWAVE", _addWaveButton);
  //  addWaveButtonMenu->addAction(_addPWave);
  //  addWaveButtonMenu->addAction(_addSWave);
  //  _addWaveButton->setMenu(addWaveButtonMenu);

  _okButton->setDisabled(true);
  // Setting`s end

  // Connecting
  connect(_infoEvent, &InfoEvent::nameChanged,
          [this](auto &name) { updateRepetition(name); });
  connect(_graphicEvent, &EventOperation::GraphicController::sendPicksInfo,
          [this](auto type, auto num, auto l_val, auto pick_val, auto r_val) {
            emit sendPicksInfo(type, num, l_val, pick_val, r_val);
          });
  connect(_graphicEvent,
          &EventOperation::GraphicController::
              createPolarizationAnalysisWindowClicked,
          [this]() { emit createPolarizationAnalysisWindow(); });
  //  connect(_polarizationEventButton, &QPushButton::clicked,
  //          [this]() { emit createPolarizationAnalysisWindow(); });
  //  connect(_okButton, &QPushButton::clicked, this, &View::accept);
  connect(_okButton, &QPushButton::clicked, [this]() {
    if (_isValid) {
      accept();
    } else {
      setNotification("There is invalid event-info");
    }
  });
  connect(_cancelButton, &QPushButton::clicked, this, &View::reject);
  //  connect(_addPWave, &QAction::triggered, [this]() {
  //    _graphicEvent->getView()->setWaveAddTriggerFlag(Data::SeismWavePick::PWAVE);
  //  });
  //  connect(_addSWave, &QAction::triggered, [this]() {
  //    _graphicEvent->getView()->setWaveAddTriggerFlag(Data::SeismWavePick::SWAVE);
  //  });
  //  connect(_clippingSlider, &QSlider::valueChanged, [this](int value) {
  //    _clippintSliderLabel->setText(
  //        QString("Clipping: %1").arg(static_cast<qreal>(value + 1) / 10));
  //    _graphicEvent->setClippingValue(static_cast<qreal>(value + 1) / 10);
  //  });
  //  connect(_gainSlider, &QSlider::valueChanged, [this](int value) {
  //    _gainSliderLabel->setText(
  //        QString("Gain: %1").arg(static_cast<qreal>(value + 1) / 10));
  //    _graphicEvent->setGainCoefficient(static_cast<qreal>(value + 1) / 10);
  //  });
  // Connecting end
}

void View::updateRepetition(const QString &name) {
  for (auto &globalName : _eventNames) {
    if (name == globalName) {
      _isValid = false;
      _infoEvent->setBrush(Qt::red);
      return;
    }
  }
  _isValid = true;
  _infoEvent->setBrush(Qt::white);
}

// void View::settingGraphicMenu() {
//  int AXIS_COUNT = 3;
//  _hideComponentsTable = new QTableWidget(this);
//  _hideComponentsTable->setColumnCount(1);
//  _hideComponentsTable->setRowCount(AXIS_COUNT);
//  _hideComponentsTable->setHorizontalHeaderLabels(QStringList() <<
//  "Component"); _hideComponentsTable->setMaximumWidth(AXIS_COUNT * 40);
//  _hideComponentsTable->setMaximumHeight(AXIS_COUNT * 38 + 1);
//  insertRowInComponentsHideTable("X", 0);
//  insertRowInComponentsHideTable("Y", 1);
//  insertRowInComponentsHideTable("Z", 2);
//  _clippintSliderLabel = new QLabel("Clipping: 1");
//  _gainSliderLabel = new QLabel("Gain: 1");
//  _clippingSlider = new QSlider(Qt::Horizontal, this);
//  _gainSlider = new QSlider(Qt::Horizontal, this);
//  _gainSlider->setTickInterval(1);
//  _gainSlider->setMaximum(100);
//  _gainSlider->setMinimum(0);
//  _gainSlider->setValue(9); // 10. from 0 to 99
//  _clippingSlider->setMinimumWidth(100);
//  _clippingSlider->setTickInterval(1);
//  _clippingSlider->setMaximum(100);
//  _clippingSlider->setMinimum(0);
//  _clippingSlider->setValue(99);
//  _clippingSlider->hide();
//  _gainSlider->hide();
//  _clippintSliderLabel->hide();
//  _gainSliderLabel->hide();
//  _hideComponentsTable->hide();
//}

// void View::showGraphicMenu() {
//  _gainSlider->show();
//  _clippingSlider->show();
//  _gainSliderLabel->show();
//  _clippintSliderLabel->show();
//  _hideComponentsTable->show();
//  _noneWiggle->show();
//  _positiveWiggle->show();
//  _negativeWiggle->show();
//}

// void View::settingWiggleButton() {
//  _noneWiggle = new QRadioButton("None Wiggle");
//  _positiveWiggle = new QRadioButton("Positive Wiggle");
//  _negativeWiggle = new QRadioButton("Negative Wiggle");
//  _noneWiggle->setChecked(true);

//  connect(_noneWiggle, &QRadioButton::clicked,
//          [this]() { _graphicEvent->setWiggle(0); });
//  connect(_positiveWiggle, &QRadioButton::clicked,
//          [this]() { _graphicEvent->setWiggle(1); });
//  connect(_negativeWiggle, &QRadioButton::clicked,
//          [this]() { _graphicEvent->setWiggle(2); });
//  _noneWiggle->hide();
//  _positiveWiggle->hide();
//  _negativeWiggle->hide();
//}

// void View::insertRowInComponentsHideTable(const QString &axis, int index) {
//  int row = index;
//  QCheckBox *checkBox = new QCheckBox();
//  checkBox->setText(axis);
//  QWidget *buttonWidget = new QWidget();
//  QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
//  buttonLayout->setAlignment(Qt::AlignLeft);
//  buttonLayout->addWidget(checkBox);
//  buttonLayout->setContentsMargins(0, 0, 0, 0);
//  _hideComponentsTable->setCellWidget(row, 0, buttonWidget);
//  checkBox->setChecked(true);
//  connect(checkBox, &QCheckBox::stateChanged, [this, index](int state) {
//    if (index == 0) {
//      if (Qt::Unchecked == state) {
//        _graphicEvent->hideAxisX(true);
//      } else if (Qt::Checked == state) {
//        _graphicEvent->hideAxisX(false);
//      }
//    }
//    if (index == 1) {
//      if (Qt::Unchecked == state) {
//        _graphicEvent->hideAxisY(true);
//      } else {
//        _graphicEvent->hideAxisY(false);
//      }
//    }
//    if (index == 2) {
//      if (Qt::Unchecked == state) {
//        _graphicEvent->hideAxisZ(true);
//      } else {
//        _graphicEvent->hideAxisZ(false);
//      }
//    }
//  });
//}

} // namespace OneEvent
} // namespace EventOperation
