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
  void
  updateProject(const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &);

  void updateProject(const std::unique_ptr<Data::SeismHorizon> &);
  void updateProjectRemoveEvent(const QUuid &);
  void updateProjectRemoveHorizon(const QUuid &);

  void updateProject(const std::unique_ptr<Data::SeismReceiver> &);
  void updateProjectRemoveReceiver(const QUuid &);

  void updateProject(const std::unique_ptr<Data::SeismWell> &);
  void updateProjectRemoveWell(const QUuid &);

  void closeProject();

signals:
  void addEventClicked() const;
  void viewEventClicked(const QUuid) const;
  void removeEventClicked(const QUuid) const;
  void processEventsClicked() const;

  void horizonsClicked() const;

  void receiversClicked() const;

  void wellsClicked() const;

  void newProjectClicked() const;
  void openProjectClicked() const;
  void saveProjectClicked() const;
  void closeProjectClicked() const;

  void projectPresence(bool) const; // NOTE: for menu bar

private:
  WorkPage *_workPage{nullptr}; // NOTE: правильно ли хранить этот указатель - ?
};

} // namespace Main
