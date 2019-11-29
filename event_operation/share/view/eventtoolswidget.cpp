#include "eventtoolswidget.h"

#include "data/seismevent.h"

#include <QBoxLayout>
#include <QPushButton>

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
EventToolsWidget::EventToolsWidget(QWidget *parent)
    : QWidget(parent), _undoButton(new QPushButton()),
      _redoButton(new QPushButton()), _dataToEBasisButton(new QPushButton()),
      _testMultButton(new QPushButton()) {

  // Setting`s
  _undoButton->setIcon(QIcon(":/icons/undo.png"));
  _redoButton->setIcon(QIcon(":/icons/redo.png"));
  _dataToEBasisButton->setIcon(QIcon(":/icons/rotate.png"));
  _testMultButton->setIcon(QIcon(":/icons/test_mult.png"));
  // Setting`s end

  // Connecting
  connect(_undoButton, &QPushButton::clicked, [this] { emit undoClicked(); });
  connect(_redoButton, &QPushButton::clicked, [this] { emit redoClicked(); });

  connect(_dataToEBasisButton, &QPushButton::clicked, [this]() {
    emit eventTransformClicked(
        SeismEvent::TransformOperation::RotateDataToEBasis);
  });
  connect(_testMultButton, &QPushButton::clicked, [this]() {
    emit eventTransformClicked(SeismEvent::TransformOperation::TestMultiplier);
  });

  // Connecting end

  // Layout`s
  QHBoxLayout *mainLayout = new QHBoxLayout();

  mainLayout->addWidget(_undoButton);
  mainLayout->addWidget(_redoButton);
  //  mainLayout->addWidget(_dataToEBasisButton); // TODO: implement!
  mainLayout->addWidget(_testMultButton);
  mainLayout->addStretch(1);

  setLayout(mainLayout);
  // Layout`s end
}

void EventToolsWidget::update(SeismEvent const *const event) {
  _dataToEBasisButton->setDisabled(
      event->isTransformBy(SeismEvent::TransformOperation::RotateDataToEBasis));
  _testMultButton->setDisabled(
      event->isTransformBy(SeismEvent::TransformOperation::TestMultiplier));
}

void EventToolsWidget::connectUndoStack(QUndoStack const *const undoStack) {
  connect(undoStack, &QUndoStack::canUndoChanged, _undoButton,
          &QPushButton::setEnabled);
  connect(undoStack, &QUndoStack::canRedoChanged, _redoButton,
          &QPushButton::setEnabled);
  _undoButton->setEnabled(undoStack->canUndo());
  _redoButton->setEnabled(undoStack->canRedo());
}

void EventToolsWidget::disconnectUndoStack(QUndoStack const *const undoStack) {
  disconnect(undoStack, &QUndoStack::canUndoChanged, _undoButton,
             &QPushButton::setEnabled);
  disconnect(undoStack, &QUndoStack::canRedoChanged, _redoButton,
             &QPushButton::setEnabled);
}

} // namespace EventOperation
