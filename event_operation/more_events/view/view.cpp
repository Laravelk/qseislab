#include "view.h"

#include "event_operation/share/view/graphiccontroller.h"
#include "event_operation/share/view/infoevent.h"

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

#include <set>

#include "data/seismevent.h" // TODO: delete
#include <iostream>          // TODO: delete

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace MoreEvents {
View::View(const std::map<QUuid, QString> &wellNames_map, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _infoEvent(new InfoEvent()), _wellNames(new QComboBox()),
      _fileDialog(new QFileDialog(this)), _eventList(new QListWidget()),
      _graphicEvent(new GraphicController(this)),
      _okButton(new QPushButton("Ok")),
      _cancelButton(new QPushButton("Cancel")),
      _addWaveButton(new QPushButton("+")),
      _polarizationEventButton(new QPushButton("Polarization Analysis")) {
  // Setting`s
  setWindowTitle("SeismWindow");
  setMinimumSize(1100, 590);

  _fileDialog->setFileMode(QFileDialog::ExistingFiles);
  _fileDialog->setOption(QFileDialog::DontResolveSymlinks);
  _fileDialog->setNameFilter("*.segy, *.sgy");

  _wellNames->addItem("Select Owner_Array...");
  for (auto &uuid_name : wellNames_map) {
    _wellNames->addItem(uuid_name.second, uuid_name.first);
  }

  _eventList->setSelectionMode(QAbstractItemView::ExtendedSelection);

  QMenu *addWaveButtonMenu = new QMenu(_addWaveButton);
  _addPWave = new QAction("PWAVE", _addWaveButton);
  _addSWave = new QAction("SWAVE", _addWaveButton);
  addWaveButtonMenu->addAction(_addPWave);
  addWaveButtonMenu->addAction(_addSWave);
  _addWaveButton->setMenu(addWaveButtonMenu);

  _infoEvent->setDisabled(true);
  _addWaveButton->setDisabled(true);
  _polarizationEventButton->setDisabled(true);
  _okButton->setDisabled(true);

  QPushButton *addEventsButton = new QPushButton("+");
  addEventsButton->setFixedWidth(25);
  QPushButton *removeEventsButton = new QPushButton("-");
  removeEventsButton->setFixedWidth(25);
  // Setting`s end

  // Connecting
  connect(addEventsButton, &QPushButton::clicked, [this]() {
    _fileDialog->open(this, SLOT(recvFilesPath(const QStringList &)));
  });
  connect(removeEventsButton, &QPushButton::clicked, [this]() {
    auto selectedEvents = _eventList->selectedItems();
    if (selectedEvents.size() == _eventList->count()) {
      _wellNames->setEnabled(true);
      _okButton->setDisabled(true);
    }

    for (auto item : selectedEvents) {
      _eventList->takeItem(_eventList->row(item)); // NOTE: bad :(
      emit removeEvent(item->data(Qt::DecorationRole).toUuid());
    }
  });

  connect(_wellNames, QOverload<int>::of(&QComboBox::activated),
          [this, addEventsButton](auto index) {
            if (0 == index) {
              _wellUuid = QUuid();
              addEventsButton->setDisabled(true);
            } else {
              _wellUuid = _wellNames->itemData(index).toUuid();
              addEventsButton->setEnabled(true);
              addEventsButton->click();
            }
          });

  connect(_eventList, &QListWidget::itemSelectionChanged, [this]() {
    auto selectedEvents = _eventList->selectedItems();
    if (1 == selectedEvents.size()) {
      _infoEvent->setEnabled(true);
      _addWaveButton->setEnabled(true);
      _polarizationEventButton->setEnabled(true);
      emit changeCurrentEvent(
          selectedEvents[0]->data(Qt::DecorationRole).toUuid());
    } else {
      _infoEvent->setDisabled(true);
      _addWaveButton->setDisabled(true);
      _polarizationEventButton->setDisabled(true);
      emit hideCurrentEvent();
    }
  });

  connect(_infoEvent, &InfoEvent::nameChanged,
          [this](auto &name) { _eventList->currentItem()->setText(name); });

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

  QHBoxLayout *changingListButtonsLayout = new QHBoxLayout();
  changingListButtonsLayout->addStretch(1);
  changingListButtonsLayout->addWidget(addEventsButton);
  changingListButtonsLayout->addWidget(removeEventsButton);

  // Layout`s
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(_infoEvent);
  leftLayout->addWidget(_wellNames);
  leftLayout->addWidget(_eventList);
  leftLayout->addLayout(changingListButtonsLayout);
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
  mainLayout->addLayout(graphicLayout, 10);

  setLayout(mainLayout);
  // Layout`s end
}

void View::loadEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  _infoEvent->update(event);
  _graphicEvent->update(event);
}

void View::unloadEvent() {
  _infoEvent->clear();
  _graphicEvent->clear();
}

void View::update(
    const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &events_map) {
  _wellNames->setDisabled(true);
  _okButton->setEnabled(true);

  std::set<QUuid> existingUuid;
  for (int i = 0; i < _eventList->count(); ++i) {
    existingUuid.insert(_eventList->item(i)->data(Qt::DecorationRole).toUuid());
  }
  for (auto &uuid_event : events_map) {
    auto &uuid = uuid_event.first;
    if (existingUuid.end() == existingUuid.find(uuid)) {
      QListWidgetItem *item = new QListWidgetItem(uuid_event.second->getName());
      item->setData(Qt::DecorationRole, uuid);
      _eventList->addItem(item);
    }
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

void View::recvFilesPath(const QStringList &paths) {
  emit sendWellUuidAndFilePaths(_wellUuid, paths);
}

} // namespace MoreEvents
} // namespace EventOperation
