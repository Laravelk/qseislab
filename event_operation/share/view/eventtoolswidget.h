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
  explicit EventToolsWidget(QWidget *parent = nullptr);

  void update(Data::SeismEvent const *const);

  void connectUndoStack(QUndoStack const *const);
  void disconnectUndoStack(QUndoStack const *const);

signals:
  void undoClicked() const;
  void redoClicked() const;

  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;
  void
      eventTransformSettingsClicked(Data::SeismEvent::TransformOperation) const;
  //  void dataToEBasisClicked() const;

private:
  QPushButton *_undoButton;
  QPushButton *_redoButton;

  QPushButton *_rotateDataButton;
  QPushButton *_testMultButton;
};
} // namespace EventOperation
