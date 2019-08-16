#pragma once

#include "data/seismproject.h"

#include <QMainWindow>
#include <QMenuBar>

#include <memory>

namespace Main {
class WorkPage;
class View : public QMainWindow {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void loadProject(const std::unique_ptr<Data::SeismProject> &);
  void updateProject(const std::unique_ptr<Data::SeismEvent> &);
  void updateProject(const std::unique_ptr<Data::SeismHorizon> &);
  void updateProjectRemoveEvent(const QUuid &);
  void updateProjectRemoveHorizon(const QUuid &);
  void closeProject();

signals:
  void addEventClicked() const;
  void viewEventClicked(const QUuid) const;
  void removeEventClicked(const QUuid) const;

  void addHorizonClicked() const;

  void newProjectClicked() const;
  void openProjectClicked() const;
  void saveProjectClicked() const;
  void closeProjectClicked() const;

  void projectPresence(bool) const; // NOTE: for menu bar

private slots:
  void handleAddEventClicked();
  void handleViewEventClicked(const QUuid);
  void handleRemoveEventClicked(const QUuid);

  void handleAddHorizonClicked();

  void handleNewProjectClicked();
  void handleOpenProjectClicked();
  void handleSaveProjectClicked();
  void handleCloseProjectClicked();

private:
  WorkPage *_workPage{nullptr}; // NOTE: правильно ли хранить этот указатель - ?
};

} // namespace Main
