#pragma once

#include "undo_stack_work/customindividualundostack.h"
#include "event_operation/share/polarizationanalysiscompute.h"

#include "view/view.h"

#include <QObject>

#include <memory>

class QUndoStack;

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class Model;
class PolarizationAnalysisWindow;
namespace OneEvent {
class View;
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(
      const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &,
      const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &,
      QObject *parent = nullptr);

  void start();
  void finish(int);

signals:
  void sendEventAndStack(std::shared_ptr<Data::SeismEvent> &,
                         std::shared_ptr<CustomIndividualUndoStack> &);
  void finished() const;

private:
  QString generateEventName() const;

  Model *_model;

  std::unique_ptr<View> _view;

  std::map<QUuid, QString> _eventNameContainer;

  PolarizationAnalysisWindow *_polarizationWindow;
  PolarizationAnalysisCompute *_calculatePolarization = nullptr;

  bool checkPolarizationAnalysisDataValid();
  bool _removedPickAndNeedUpdatePolarGraph = false;



  std::shared_ptr<Data::SeismEvent> _event;
  std::shared_ptr<CustomIndividualUndoStack> _undoStack;
};

} // namespace OneEvent
} // namespace EventOperation
