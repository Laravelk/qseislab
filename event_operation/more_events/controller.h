#pragma once

<<<<<<< HEAD
#include "undo_stack_work/customindividualundostack.h"
#include "view/view.h"

#include <QUndoCommand>
#include <QUuid>

=======
#include "view/view.h"

#include <QObject>

#include <QUuid>
>>>>>>> test
#include <map>
#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class Model;
class PolarizationAnalysisWindow;
<<<<<<< HEAD
=======
class PolarizationAnalysisCompute;
>>>>>>> test
namespace MoreEvents {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(
<<<<<<< HEAD
      const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &,
      const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &,
=======
      const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &,
      const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &,
>>>>>>> test
      QObject *parent = nullptr);

  void start();
  void finish(int);

signals:
<<<<<<< HEAD
  void sendEventsAndStacks(
      std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &,
      std::map<QUuid, std::shared_ptr<CustomIndividualUndoStack>> &) const;
=======
  void sendEvents(std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &) const;
>>>>>>> test
  void finished() const;

private:
  Model *_model;

  std::unique_ptr<View> _view;

<<<<<<< HEAD
  std::map<QUuid, std::shared_ptr<Data::SeismEvent>> _events_map;
  std::map<QUuid, std::shared_ptr<CustomIndividualUndoStack>> _stacks_map;

  QUuid _currentEventUuid;

  PolarizationAnalysisWindow *_polarizationWindow;
=======
  std::map<QUuid, std::unique_ptr<Data::SeismEvent>> _events_map;

  QUuid _currentEventUuid;

  PolarizationAnalysisWindow *_polarizationWindow = nullptr;
  PolarizationAnalysisCompute *_calculatePolarization = nullptr; // TODO: delete
>>>>>>> test
};

} // namespace MoreEvents
} // namespace EventOperation
