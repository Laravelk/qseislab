#pragma once

#include "undo_stack_work/customindividualundostack.h"

#include <QObject>

#include <memory>

class QUndoStack;

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class View;
class PolarizationAnalysisWindow;
namespace ViewEvent {
class View;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(
      const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &,
      const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &,
      const std::shared_ptr<Data::SeismEvent> &, QUndoStack const *const,
      QObject *parent = nullptr);

  QWidget *getView();

signals:
  void undoClicked(const QUuid &) const;
  void redoClicked(const QUuid &) const;
  void eventActionClicked(const QUuid &,
                          Data::SeismEvent::TransformOperation) const;

  void finished() const;

private:
  View *_view;

  std::map<QUuid, QString> _eventNameContainer;

  PolarizationAnalysisWindow *_polarizationWindow;

  std::shared_ptr<Data::SeismEvent> _event;
};

} // namespace ViewEvent
} // namespace EventOperation
