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
      const std::shared_ptr<Data::SeismEvent> &, QObject *parent = nullptr);

  QWidget *getView();

signals:
  void finished() const;

private:
  View *_view;

  std::map<QUuid, QString> _eventNameContainer;

  PolarizationAnalysisWindow *_polarizationWindow;

  std::shared_ptr<Data::SeismEvent> _event;
};

} // namespace ViewEvent
} // namespace EventOperation
