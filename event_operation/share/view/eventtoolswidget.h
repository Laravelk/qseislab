#pragma once

#include "data/seismevent.h"

#include <QWidget>

#include <QPushButton>
#include <QUndoStack>
#include <memory>

namespace EventOperation {
class EventToolsWidget : public QWidget {
  Q_OBJECT

public:
  explicit EventToolsWidget(QUndoStack const *const, QWidget *parent = nullptr);
  explicit EventToolsWidget(QWidget *parent = nullptr);

  void update(Data::SeismEvent const *const);

signals:
  void undoClicked() const;
  void redoClicked() const;

  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;

private:
  QPushButton *_undoButton;
  QPushButton *_redoButton;

  QPushButton *_rotateDataButton;
  QPushButton *_testMultButton;

  QPushButton *_ffilteringButton;
};
} // namespace EventOperation
