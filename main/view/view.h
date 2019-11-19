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

  void viewAboutProject(const std::unique_ptr<Data::SeismProject> &);

  void updateUndoStack(const std::shared_ptr<QUndoStack> &);

  void loadProject(const std::unique_ptr<Data::SeismProject> &);
  void addEvent(const std::shared_ptr<Data::SeismEvent> &);
  void
  processedEvents(const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &);
  void updateEvent(const std::shared_ptr<Data::SeismEvent> &);
  void removeEvent(const QUuid &);

  void addHorizon(const std::shared_ptr<Data::SeismHorizon> &);
  void removeHorizon(const QUuid &);

  void addWell(const std::shared_ptr<Data::SeismWell> &);
  void removeWell(const QUuid &);

  void
  addReceiver(const std::shared_ptr<Data::SeismReceiver> &); // TODO: re-see
  void removeReceiver(const QUuid &);                        // TODO: re-see

  void closeProject();

signals:
  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;

  void changeEventFocus(const std::set<QUuid> &) const;
  void undoClicked() const;
  void redoClicked() const;

  void addEventsClicked() const;
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
  void aboutProjectClicked() const;

  void projectPresence(bool) const; // NOTE: for menu bar

private:
  WorkPage *_workPage{nullptr}; // NOTE: правильно ли хранить этот указатель - ?
};

} // namespace Main
