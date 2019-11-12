#include "view.h"

#include "event_operation/share/view/eventtoolswidget.h"
#include "event_operation/share/view/graphiccontroller.h"
#include "event_operation/share/view/infoevent.h"

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

#include <set>

#include "data/seismevent.h"
#include <iostream> // TODO: delete

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace MoreEvents {
View::View(const std::set<QString> &globalEventNames,
           const std::map<QUuid, QString> &wellNames_map, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _toolsWidget(new EventToolsWidget()), _infoEvent(new InfoEvent()),
      _wellNames(new QComboBox()), _fileDialog(new QFileDialog(this)),
      _eventList(new QListWidget()), _graphicEvent(new GraphicController(this)),
      _okButton(new QPushButton("Ok")),
      _cancelButton(new QPushButton("Cancel")),
      _globalEventNames(globalEventNames) {

  // Setting`s
  setWindowTitle("SeismWindow");
  setMinimumSize(1300, 590);

  _toolsWidget->setDisabled(true);

  _fileDialog->setFileMode(QFileDialog::ExistingFiles);
  _fileDialog->setOption(QFileDialog::DontResolveSymlinks);
  _fileDialog->setNameFilter("*.segy *.sgy");

  _wellNames->addItem("Select Owner_Array...");
  for (auto &uuid_name : wellNames_map) {
    _wellNames->addItem(uuid_name.second, uuid_name.first);
  }

  _eventList->setDragEnabled(false);
  _eventList->setAcceptDrops(false);
  _eventList->setSortingEnabled(false);
  _eventList->setSelectionMode(QAbstractItemView::ExtendedSelection);

  _infoEvent->setDisabled(true);
  _okButton->setDisabled(true);
  _cancelButton->setFocus();

  QPushButton *addEventsButton = new QPushButton("+");
  addEventsButton->setDisabled(true);
  //  addEventsButton->setFixedWidth(25);
  QPushButton *removeEventsButton = new QPushButton("-");
  //  removeEventsButton->setFixedWidth(25);
  // Setting`s end

  // Connecting
  connect(_toolsWidget, &EventToolsWidget::eventTransformClicked,
          [this](auto oper) { emit eventTransformClicked(oper); });
  connect(addEventsButton, &QPushButton::clicked, [this]() {
    _fileDialog->open(this, SLOT(recvFilesPath(const QStringList &)));
  });
  connect(removeEventsButton, &QPushButton::clicked, [this]() {
    auto selectedEvents = _eventList->selectedItems();
    if (selectedEvents.size() == _eventList->count()) {
      _wellNames->setEnabled(true);
      _okButton->setDisabled(true);
      _cancelButton->setFocus();
    }

    int nextFocusRow = 0;
    int selectedCount = selectedEvents.size();
    int allCount = _eventList->count();
    if (1 != selectedCount) {
      nextFocusRow = allCount - selectedCount - 1;
    }

    for (auto item : selectedEvents) {
      auto name = item->text();
      removeLocal(name);
      updateRepetition(name);

      int row = _eventList->row(item);
      if (1 == selectedCount) {
        nextFocusRow = (allCount - 1 == row) ? row - 1 : row;
      }

      _eventList->takeItem(row);
      emit removeEvent(item->data(Qt::DecorationRole).toUuid());
    }
    _eventList->setCurrentRow(nextFocusRow);
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
      emit changeCurrentEvent(
          selectedEvents[0]->data(Qt::DecorationRole).toUuid());
    } else {
      emit hideCurrentEvent();
    }
  });

  connect(_infoEvent, &InfoEvent::nameChanged, [this](auto &name) {
    auto item = _eventList->selectedItems()[0];
    auto oldName = item->text();
    removeLocal(oldName);
    updateRepetition(oldName);

    addLocal(name);
    item->setText(name);
    _infoEvent->setBrush(updateRepetition(name));

    _graphicEvent->updateEventName(name);
  });

  connect(_graphicEvent, &EventOperation::GraphicController::sendPicksInfo,
          [this](auto type, auto num, auto l_val, auto pick_val, auto r_val) {
            emit sendPicksInfo(type, num, l_val, pick_val, r_val);
          });
  connect(_graphicEvent,
          &EventOperation::GraphicController::
              createPolarizationAnalysisWindowClicked,
          [this]() { emit createPolarizationAnalysisWindow(); });
  connect(_okButton, &QPushButton::clicked, [this]() {
    if (allValid()) {
      accept();
    } else {
      setNotification("There are invalid events-info");
    }
  });
  connect(_cancelButton, &QPushButton::clicked, this, &View::reject);
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
  _undoButton = new QPushButton("Undo");
  _redoButton = new QPushButton("Redo");
  _undoButton->setDisabled(true);
  _redoButton->setDisabled(true);
  //  connect(undoStack, &QUndoStack::canUndoChanged, undoButton,
  //  &QPushButton::setEnabled); connect(undoStack, &QUndoStack::canRedoChanged,
  //  redoButton, &QPushButton::setEnabled);
  connect(_undoButton, &QPushButton::clicked, [this]() { emit undoClicked(); });
  connect(_redoButton, &QPushButton::clicked, [this]() { emit redoClicked(); });
  buttonsLayout->addWidget(_undoButton);
  buttonsLayout->addWidget(_redoButton);

  buttonsLayout->addWidget(_toolsWidget);
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(_okButton);
  buttonsLayout->addWidget(_cancelButton);

  QVBoxLayout *graphicLayout = new QVBoxLayout();
  graphicLayout->addWidget(_graphicEvent->getView(), 10);
  graphicLayout->addStretch(1);
  //  graphicLayout->addLayout(buttonsLayout);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  mainLayout->addLayout(graphicLayout, 10);

  QVBoxLayout *mainButtonLayout = new QVBoxLayout();
  mainButtonLayout->addLayout(mainLayout);
  mainButtonLayout->addStretch(1);
  mainButtonLayout->addLayout(buttonsLayout);

  setLayout(mainButtonLayout);
  // Layout`s end
}

void View::loadEvent(const std::unique_ptr<Data::SeismEvent> &event,
                     const std::unique_ptr<QUndoStack> &undoStack) {
  connect(undoStack.get(), &QUndoStack::canUndoChanged, _undoButton,
          &QPushButton::setEnabled);
  connect(undoStack.get(), &QUndoStack::canRedoChanged, _redoButton,
          &QPushButton::setEnabled);
  _undoButton->setEnabled(undoStack->canUndo());
  _redoButton->setEnabled(undoStack->canRedo());

  _toolsWidget->setEnabled(true);
  _toolsWidget->update(event);

  _infoEvent->setEnabled(true);
  _infoEvent->update(event);
  _graphicEvent->update(event);
}

void View::unloadEvent(const std::unique_ptr<QUndoStack> &undoStack) {
  disconnect(undoStack.get(), &QUndoStack::canUndoChanged, _undoButton,
             &QPushButton::setEnabled);
  disconnect(undoStack.get(), &QUndoStack::canRedoChanged, _redoButton,
             &QPushButton::setEnabled);

  _undoButton->setDisabled(true);
  _redoButton->setDisabled(true);

  _toolsWidget->setDisabled(true);

  _infoEvent->clear();
  _graphicEvent->clear();

  _infoEvent->setDisabled(true);
}

void View::update(const std::unique_ptr<Data::SeismEvent> &event) {
  _toolsWidget->update(event);
  _graphicEvent->update(event);
  // NOTE: надо _eventList обновлять?
}

void View::update(
    const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &events_map) {
  _wellNames->setDisabled(true);
  _okButton->setEnabled(true);
  _okButton->setFocus();

  std::set<QUuid> existingUuid;
  for (int i = 0; i < _eventList->count(); ++i) {
    existingUuid.insert(_eventList->item(i)->data(Qt::DecorationRole).toUuid());
  }
  for (auto &uuid_event : events_map) {
    auto &uuid = uuid_event.first;
    if (existingUuid.end() == existingUuid.find(uuid)) {
      auto &name = uuid_event.second->getName();
      QListWidgetItem *item = new QListWidgetItem(name);
      item->setData(Qt::DecorationRole, uuid);
      _eventList->addItem(item);

      addLocal(name);
      updateRepetition(name);
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

void View::addLocal(const QString &name) {
  if (_localEventNames.end() == _localEventNames.find(name)) {
    _localEventNames[name] = 1;
  } else {
    _localEventNames[name] += 1;
  }
}

void View::removeLocal(const QString &name) { _localEventNames[name] -= 1; }

QBrush View::updateRepetition(const QString &name) {
  QBrush brush(Qt::white);
  if (_globalEventNames.end() != _globalEventNames.find(name)) {
    brush.setColor(Qt::red);
    for (int i = 0; i < _eventList->count(); ++i) {
      auto item = _eventList->item(i);
      if (name == item->text()) {
        item->setBackground(brush);
      }
    }
  } else {
    if (1 != _localEventNames.at(name)) {
      brush.setColor(Qt::yellow);
      for (int i = 0; i < _eventList->count(); ++i) {
        auto item = _eventList->item(i);
        if (name == item->text()) {
          item->setBackground(brush);
        }
      }
    } else {
      for (int i = 0; i < _eventList->count(); ++i) {
        auto item = _eventList->item(i);
        if (name == item->text()) {
          item->setBackground(brush);
        }
      }
    }
  }
  return brush;
}

bool View::allValid() const {
  QBrush validBrush(Qt::white);
  for (int i = 0; i < _eventList->count(); ++i) {
    auto item = _eventList->item(i);
    if (validBrush != item->background()) {
      return false;
    }
  }
  return true;
}

} // namespace MoreEvents
} // namespace EventOperation
