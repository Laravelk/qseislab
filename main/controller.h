#pragma once

#include "data/seismevent.h"
#include "data/seismproject.h"
#include "undo_stack_work/customindividualundostack.h"
#include "view/view.h"

#include "event_operation/more_events/controller.h"
#include "event_operation/one_event/controller.h"
#include "event_operation/view_event/controller.h"

#include "horizon_operation/controller.h"

#include "receiver_operation/controller.h"

#include "well_operation/controller.h"

#include "project_operation/close_project/controller.h"
#include "project_operation/new_project/controller.h"
#include "project_operation/open_project/controller.h"
#include "project_operation/project_settings/settingdialog.h"
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
  void recvProject(const std::shared_ptr<Data::SeismProject> &);

  void
      handleEventTransformSettingsClicked(Data::SeismEvent::TransformOperation);

  void handleAddEventsClicked();
  void handleAddEventClicked();
  void handleViewEventClicked(const QUuid &);

  void handleHorizonsClicked();

  void handleReceiversClicked();

  void handleWellsClicked();

  void handleCloseProjectClicked();
  void handleNewProjectClicked();
  void handleOpenProjectClicked();
  void handleSaveProjectClicked();

  void deleteCloseProjectController(bool);

private:
  ProjectOperation::SettingDialog *
      getSettingDialog(Data::SeismEvent::TransformOperation) const;

  std::shared_ptr<Data::SeismProject> _project;

  std::map<Data::SeismEvent::TransformOperation,
           std::unique_ptr<ProjectOperation::SettingDialog>>
      _projectSettingViews;

  QUuid _currentOneEventFocus;
  std::map<QUuid, std::shared_ptr<CustomIndividualUndoStack>> _eventStacks;
  std::shared_ptr<QUndoStack> _shareEventStack;

  std::unique_ptr<View> _mainWindow;

  std::unique_ptr<EventOperation::OneEvent::Controller> _oneAddEventController;

  std::map<QUuid, std::unique_ptr<EventOperation::ViewEvent::Controller>>
      _oneViewEventControllers;

  std::unique_ptr<EventOperation::MoreEvents::Controller> _moreEventsController;

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
