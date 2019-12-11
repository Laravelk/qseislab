#include "eventtoolswidget.h"

#include "data/seismevent.h"

#include <QBoxLayout>
#include <QPushButton>

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
EventToolsWidget::EventToolsWidget(QWidget *parent)
    : QWidget(parent), _undoButton(new QPushButton()),
      _redoButton(new QPushButton()), _rotateDataButton(new QPushButton()),
      _testMultButton(new QPushButton()),
    _ffilteringButton(new QPushButton()){

  // Setting`s
  _undoButton->setIcon(QIcon(":/icons/undo.png"));
  _redoButton->setIcon(QIcon(":/icons/redo.png"));
  _rotateDataButton->setIcon(QIcon(":/icons/rotate.png"));
  _testMultButton->setIcon(QIcon(":/icons/test_mult.png"));
  _ffilteringButton->setText("FFiltering");

  /* test for set setttings */
  QPushButton *testMultSettingsButton = new QPushButton("test_mult_set");
  // Setting`s end

  // Connecting
  connect(_undoButton, &QPushButton::clicked, [this] { emit undoClicked(); });
  connect(_redoButton, &QPushButton::clicked, [this] { emit redoClicked(); });

  connect(_rotateDataButton, &QPushButton::clicked, [this]() {
    emit eventTransformClicked(SeismEvent::TransformOperation::RotateData);
  });
  connect(_testMultButton, &QPushButton::clicked, [this]() {
    emit eventTransformClicked(SeismEvent::TransformOperation::TestMultiplier);
  });
  connect(_ffilteringButton, &QPushButton::clicked, [this]{
      emit eventTransformClicked(SeismEvent::TransformOperation::FFilteringData);
  });

  /* test for set setttings */
  connect(testMultSettingsButton, &QPushButton::clicked, [this]() {
    emit eventTransformSettingsClicked(
        SeismEvent::TransformOperation::TestMultiplier);
  });

  // Connecting end

  // Layout`s
  QHBoxLayout *mainLayout = new QHBoxLayout();

  mainLayout->addWidget(_undoButton);
  mainLayout->addWidget(_redoButton);
  //  mainLayout->addWidget(_dataToEBasisButton); // TODO: implement!
  mainLayout->addWidget(_testMultButton);
  mainLayout->addWidget(testMultSettingsButton);
  mainLayout->addWidget(_ffilteringButton);
  mainLayout->addStretch(1);

  setLayout(mainLayout);
  // Layout`s end
}

void EventToolsWidget::update(SeismEvent const *const event) {
  _rotateDataButton->setDisabled(
      event->isTransformBy(SeismEvent::TransformOperation::RotateData));
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
