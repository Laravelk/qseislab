#pragma once

#include "view/view.h"

#include <QUndoCommand>
#include <QUuid>

#include <map>
#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
class SeismReceiver;
class ProjectSettings;
} // namespace Data

namespace EventOperation {
class Model;
class PolarizationAnalysisWindow;
class PolarizationAnalysisCompute;
class AnalysisWindow;
namespace MoreEvents {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(
      const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &,
      const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &,
      const std::list<std::shared_ptr<Data::SeismReceiver>> &,
      Data::ProjectSettings *const, QObject *parent = nullptr);

  void start();
  void finish(int);

signals:
  void sendEventsAndStack(std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &,
                          std::shared_ptr<QUndoStack> &) const;

  void finished() const;

private slots:
  void handleUndoClicked();
  void handleRedoClicked();

private:
  Model *_model;

  std::unique_ptr<View> _view;

  bool checkPolarizationAnalysisDataValid();

  std::map<QUuid, std::shared_ptr<Data::SeismEvent>> _events_map;

  std::shared_ptr<QUndoStack> _undoStack;

  QUuid _currentEventUuid;

  //  PolarizationAnalysisWindow *_polarizationWindow = nullptr;
  AnalysisWindow *_analysisWindow = nullptr;
};

} // namespace MoreEvents
} // namespace EventOperation
