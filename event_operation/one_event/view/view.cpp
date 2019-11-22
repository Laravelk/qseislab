#include "view.h"

#include "event_operation/share/view/eventtoolswidget.h"
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
// View::View(const std::set<QString> &eventNames, SeismEvent const *const
// event,
//           QUndoStack const *const undoStack, QWidget *parent)
//    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
//      _eventNames(eventNames) {

//  commonSetting();
//  //  _toolsWidget->update(event);
//  //  _toolsWidget->setEnabled(true);
//  _infoEvent->setEnabled(true);
//  //  _infoEvent->update(event);
//  //  _graphicEvent->update(event);
//  _okButton->setEnabled(true);
//  update(event);

//  // Layout`s
//  QVBoxLayout *leftLayout = new QVBoxLayout();
//  leftLayout->addWidget(_infoEvent);
//  leftLayout->addStretch(1);

//  QHBoxLayout *buttonsLayout = new QHBoxLayout();
//  //  buttonsLayout->addWidget(new QUndoView(undoStack)); // NOTE: undo/redo -
//  //  view TODO: re-build
//  auto undoButton = new QPushButton("Undo");
//  auto redoButton = new QPushButton("Redo");
//  undoButton->setEnabled(undoStack->canUndo());
//  redoButton->setEnabled(undoStack->canRedo());
//  //  undoButton->setDisabled(true);
//  //  redoButton->setDisabled(true);
//  connect(undoStack, &QUndoStack::canUndoChanged, undoButton,
//          &QPushButton::setEnabled);
//  connect(undoStack, &QUndoStack::canRedoChanged, redoButton,
//          &QPushButton::setEnabled);
//  connect(undoButton, &QPushButton::clicked, [this]() { emit undoClicked();
//  }); connect(redoButton, &QPushButton::clicked, [this]() { emit
//  redoClicked(); }); buttonsLayout->addWidget(undoButton);
//  buttonsLayout->addWidget(redoButton);

//  buttonsLayout->addWidget(_toolsWidget);
//  buttonsLayout->addStretch(1);
//  buttonsLayout->addWidget(_okButton);
//  buttonsLayout->addWidget(_cancelButton);

//  //  QVBoxLayout *graphicLayout = new QVBoxLayout();
//  //  graphicLayout->addWidget(_graphicEvent->getView(), 10);
//  //  graphicLayout->addStretch(1);
//  //  graphicLayout->addLayout(buttonsLayout);

//  QHBoxLayout *mainLayout = new QHBoxLayout();
//  mainLayout->addLayout(leftLayout);
//  mainLayout->addWidget(_graphicEvent->getView(), 10);

//  QVBoxLayout *mainButtonLayout = new QVBoxLayout();
//  mainButtonLayout->addLayout(mainLayout);
//  mainButtonLayout->addStretch(1);
//  mainButtonLayout->addLayout(buttonsLayout);

//  setLayout(mainButtonLayout);
//  // Layout`s end
//}

View::View(const std::set<QString> &eventNames,
           const std::map<QUuid, QString> &wellNames_map,
           QUndoStack const *const undoStack, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _wellManagersLayout(new QVBoxLayout()),
      _addButtonManagers(new QPushButton("Add")), _wellNames_map(wellNames_map),
      _eventNames(eventNames) {

  commonSetting();

  // Setting`s
  _toolsWidget->setDisabled(true);

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
  //  buttonsLayout->addWidget(new QUndoView(undoStack)); // NOTE: undo/redo -
  //  view TODO: re-build

  auto undoButton = new QPushButton("Undo");
  auto redoButton = new QPushButton("Redo");
  //  undoButton->setDisabled(true);
  //  redoButton->setDisabled(true);
  undoButton->setEnabled(undoStack->canUndo());
  redoButton->setEnabled(undoStack->canRedo());
  connect(undoStack, &QUndoStack::canUndoChanged, undoButton,
          &QPushButton::setEnabled);
  connect(undoStack, &QUndoStack::canRedoChanged, redoButton,
          &QPushButton::setEnabled);
  connect(undoButton, &QPushButton::clicked, [this]() { emit undoClicked(); });
  connect(redoButton, &QPushButton::clicked, [this]() { emit redoClicked(); });
  buttonsLayout->addWidget(undoButton);
  buttonsLayout->addWidget(redoButton);

  buttonsLayout->addWidget(_toolsWidget);
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(_okButton);
  buttonsLayout->addWidget(_cancelButton);

  //  QVBoxLayout *graphicLayout = new QVBoxLayout();
  //  graphicLayout->addWidget(_graphicEvent->getView(), 10);
  //  graphicLayout->addStretch(1);
  //  graphicLayout->addLayout(buttonsLayout);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  mainLayout->addStretch(1);
  mainLayout->addWidget(_graphicEvent, 10);

  // setting tool-bar
  //  QToolBar* toolBar = new QToolBar();
  //  toolBar->addWidget(_toolsWidget);
  //  QAction* action = toolBar->addAction("");

  // end of setting tool-bar

  QVBoxLayout *mainButtonLayout = new QVBoxLayout();
  //  mainButtonLayout->addWidget(toolBar);
  mainButtonLayout->addLayout(mainLayout);
  mainButtonLayout->addStretch(1);
  mainButtonLayout->addLayout(buttonsLayout);

  setLayout(mainButtonLayout);
  // Layout`s end
}

void View::update(SeismEvent const *const event) {
  _toolsWidget->update(event);
  _infoEvent->update(event);
  _graphicEvent->update(event);
}

void View::update(SeismEvent const *const event, const QUuid &removedWellUuid) {
  _wellNames_map.erase(removedWellUuid);

  _toolsWidget->setEnabled(true);
  _toolsWidget->update(event);

  _infoEvent->setEnabled(true);
  _infoEvent->update(event);
  _graphicEvent->update(event);
  _okButton->setEnabled(true);
  _okButton->setFocus();
}

void View::update(SeismEvent const *const event, const QUuid &uuid,
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
    _toolsWidget->setDisabled(true);

    _infoEvent->clear();
    _infoEvent->setDisabled(true);

    _graphicEvent->clear();

    _okButton->setDisabled(true);
    _cancelButton->setFocus();
  } else {
    _toolsWidget->setEnabled(true);

    _infoEvent->setEnabled(true);
    _infoEvent->update(event);
    _graphicEvent->update(event);
    _okButton->setEnabled(true);
    _okButton->setFocus();
  }

  _toolsWidget->update(event); // TODO: надо ли и сбрасываются ли применения?
}

void View::setNotification(const QString &text) {
  QMessageBox *msg = new QMessageBox(QMessageBox::Critical, "Message", text,
                                     QMessageBox::Ok, this);
  msg->show();
}

void View::settingEventInfo(SeismEvent *const event) const {
  _infoEvent->settingEventInfo(event);
}

ChartGesture *View::getChartGesture() { return _graphicEvent->getModel(); }

void View::commonSetting() {
  // Setting`s
  _toolsWidget = new EventToolsWidget(this);
  _infoEvent = new InfoEvent(this);
  _graphicEvent = new GraphicController(this);
  _okButton = new QPushButton("Ok", this);
  _cancelButton = new QPushButton("Cancel", this);

  setWindowTitle("SeismWindow");
  setMinimumSize(1300, 590);

  //  _toolsWidget->setDisabled(true);

  _infoEvent->setDisabled(true);

  _okButton->setDisabled(true);
  _cancelButton->setFocus();
  // Setting`s end

  // Connecting
  connect(_toolsWidget, &EventToolsWidget::eventTransformClicked,
          [this](auto oper) { emit eventTransformClicked(oper); });
  connect(_infoEvent, &InfoEvent::nameChanged, [this](auto &name) {
    updateRepetition(name);
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
    if (_isValid) {
      accept();
    } else {
      setNotification("There is invalid event-info");
    }
  });
  connect(_cancelButton, &QPushButton::clicked, this, &View::reject);

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

} // namespace OneEvent
} // namespace EventOperation
