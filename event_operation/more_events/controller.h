#pragma once

#include "undo_stack_work/customindividualundostack.h"
#include "view/view.h"

#include <QUndoCommand>
#include <QUuid>

#include <map>
#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
class SeismReceiver;
} // namespace Data

namespace EventOperation {
class Model;
class PolarizationAnalysisWindow;
class PolarizationAnalysisCompute;
namespace MoreEvents {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(
      const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &,
      const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &,
      const std::list<std::shared_ptr<Data::SeismReceiver>> &,
      QObject *parent = nullptr);

  void start();
  void finish(int);

signals:
  void sendEventsAndStacks(
      std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &,
      std::map<QUuid, std::shared_ptr<CustomIndividualUndoStack>> &) const;
  void finished() const;

private:
  Model *_model;

  std::unique_ptr<View> _view;

  bool checkPolarizationAnalysisDataValid();
  bool _removedPickAndNeedUpdatePolarGraph = false;

  std::map<QUuid, std::shared_ptr<Data::SeismEvent>> _events_map;
  std::map<QUuid, std::shared_ptr<CustomIndividualUndoStack>> _stacks_map;

  QUuid _currentEventUuid;

  PolarizationAnalysisWindow *_polarizationWindow = nullptr;
  PolarizationAnalysisCompute *_calculatePolarization = nullptr;
};

} // namespace MoreEvents
} // namespace EventOperation
