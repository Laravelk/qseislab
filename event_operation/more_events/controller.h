#pragma once

#include "view/view.h"

#include <QUndoCommand>
#include <QUuid>

#include <map>
#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class Model;
class PolarizationAnalysisWindow;
namespace MoreEvents {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(
      const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &,
      const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &,
      QObject *parent = nullptr);

  void start();
  void finish(int);

signals:
  //  void sendEvents(std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &)
  //  const;
  void
  sendEventsAndStacks(std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &,
                      std::map<QUuid, std::unique_ptr<QUndoStack>> &) const;
  void finished() const;

private:
  Model *_model;

  std::unique_ptr<View> _view;

  std::map<QUuid, std::unique_ptr<Data::SeismEvent>> _events_map;
  std::map<QUuid, std::unique_ptr<QUndoStack>> _stacks_map;

  QUuid _currentEventUuid;

  PolarizationAnalysisWindow *_polarizationWindow;

  //  QUndoStack* _appliedOperations;
};

} // namespace MoreEvents
} // namespace EventOperation
