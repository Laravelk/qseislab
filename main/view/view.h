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

  void viewAboutProject(Data::SeismProject const *const);

  void updateUndoStack(QUndoStack const *const);

  void addEventPage(QWidget *, Data::SeismEvent const *const);
  void setFocusEventPage(QWidget *);

  void loadProject(Data::SeismProject const *const);
  void addEvent(Data::SeismEvent const *const);
  void
  processedEvents(const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &);
  void updateEvent(Data::SeismEvent const *const);
  void removeEvent(const QUuid &);

  void addHorizon(Data::SeismHorizon const *const);
  void removeHorizon(const QUuid &);

  void addWell(Data::SeismWell const *const);
  void removeWell(const QUuid &);

  void addReceiver(Data::SeismReceiver const *const); // TODO: re-see
  void removeReceiver(const QUuid &);                 // TODO: re-see

  void closeProject();

signals:
  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;

  void eventPageChanged(const QUuid &) const;
  void eventPageClosed(const QUuid &);

  void changeEventFocus(const std::set<QUuid> &) const;
  void changeEventFocusToOne(const QUuid &) const;

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

  QUndoStack const *_currentUndoStack{nullptr};
  QAction *_undoAction;
  QAction *_redoAction;
};

} // namespace Main
