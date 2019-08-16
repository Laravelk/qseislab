#pragma once

#include "data/seismevent.h"
#include "data/seismproject.h"
#include "view/view.h"

#include "event_operation/add_event/controller.h"
#include "event_operation/view_event/controller.h"

#include "horizon_operation/add_horizon/controller.h"

#include "project_operation/close_project/controller.h"
#include "project_operation/new_project/controller.h"
#include "project_operation/open_project/controller.h"
#include "project_operation/save_project/controller.h"

#include <QObject>

#include <memory>

namespace Main {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller();

signals:
  void savedProject(bool) const;

private slots:
  void recvProject(std::unique_ptr<Data::SeismProject> &);
  void recvEvent(std::unique_ptr<Data::SeismEvent> &);
  void recvHorizon(std::unique_ptr<Data::SeismHorizon> &);

  void updateProject(const std::unique_ptr<Data::SeismEvent> &);
  void updateProjectRemoveEvent(const QUuid &);
  void updateProject(const std::unique_ptr<Data::SeismHorizon> &);
  void updateProjectRemoveHorizon(const QUuid &);

  void handleAddEventClicked();
  void handleViewEventClicked(const QUuid);
  void handleRemoveEventClicked(const QUuid);

  void handleAddHorizonClicked();

  void handleCloseProjectClicked();
  void handleNewProjectClicked();
  void handleOpenProjectClicked();
  void handleSaveProjectClicked();

  void deleteAddEventController();
  void deleteViewEventController();

  void deleteAddHorizonController();

  void deleteCloseProjectController(bool);
  void deleteNewProjectController();
  void deleteOpenProjectController();
  void deleteSaveProjectController(bool);

private:
  std::unique_ptr<Data::SeismProject> _project;

  std::unique_ptr<View> _mainWindow;

  std::unique_ptr<EventOperation::AddEvent::Controller> _addEventController;
  std::unique_ptr<EventOperation::ViewEvent::Controller> _viewEventController;

  std::unique_ptr<HorizonOperation::AddHorizon::Controller>
      _addHorizonController;

  std::unique_ptr<ProjectOperation::CloseProject::Controller>
      _closeProjectController;
  std::unique_ptr<ProjectOperation::NewProject::Controller>
      _newProjectController;
  std::unique_ptr<ProjectOperation::OpenProject::Controller>
      _openProjectController;
  std::unique_ptr<ProjectOperation::SaveProject::Controller>
      _saveProjectController;
};

} // namespace Main
