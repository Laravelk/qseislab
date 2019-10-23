#pragma once

#include "data/seismevent.h"
#include "data/seismproject.h"
#include "view/view.h"

#include "event_operation/controller.h"

#include "horizon_operation/controller.h"

#include "receiver_operation/controller.h"

#include "well_operation/controller.h"

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
  explicit Controller(QObject *parent = nullptr);

signals:
  void savedProject(bool) const;

private slots:
  void recvProject(std::unique_ptr<Data::SeismProject> &);

  void handleAddEventClicked();
  void handleViewEventClicked(const QUuid);

  void handleHorizonsClicked();

  void handleReceiversClicked();

  void handleWellsClicked();

  void handleCloseProjectClicked();
  void handleNewProjectClicked();
  void handleOpenProjectClicked();
  void handleSaveProjectClicked();

  void deleteCloseProjectController(bool);

private:
  std::unique_ptr<Data::SeismProject> _project;

  std::unique_ptr<View> _mainWindow;

  std::unique_ptr<EventOperation::Generic::Controller>
      _eventController; // TODO: remove extra
  //  std::unique_ptr<EventOperation::AddEvent::Controller> _addEventController;
  //  std::unique_ptr<EventOperation::ViewEvent::Controller>
  //  _viewEventController;

  std::unique_ptr<HorizonOperation::Controller> _horizonController;

  std::unique_ptr<ReceiverOperation::Controller> _receiverController;

  std::unique_ptr<WellOperation::Controller> _wellController;

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
