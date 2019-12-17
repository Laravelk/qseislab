#pragma once

#include "data/seismproject.h"

//#include <QCheckBox>
#include <QFrame>
#include <QTableWidget>
//#include <QtDataVisualization>
#include "event_operation/view_event/view.h"

#include <memory>

// using namespace QtDataVisualization;

class FilteringTableAssistant;

namespace Main {
class InfoProject;
// class OilFieldScene;
class OilFieldWidget;
class WorkPage : public QFrame {
  Q_OBJECT

public:
  explicit WorkPage(QWidget *parent = nullptr);

  void loadProject(Data::SeismProject const *const);

  void addEventPage(QWidget *, Data::SeismEvent const *const);
  void closeEventPage(const QUuid &);
  void setFocusEventPage(QWidget *);
  //  QUuid getFocusEventPage() const;

  //  const QUuid getFocusEvent() const;

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

signals:
  //  void eventSelectionChanged(const std::set<QUuid> &) const;

  //  void eventActionClicked(const QUuid &,
  //                          Data::SeismEvent::TransformOperation) const;

  void eventsActionClicked(const std::set<QUuid> &,
                           Data::SeismEvent::TransformOperation) const;

  void eventPageChanged(const QUuid &) const;
  void eventPageClosed(const QUuid &);

  void viewEventClicked(const QUuid &) const;
  void removeEventClicked(const QUuid &) const;

private:
  QTabWidget *_workPages;
  std::map<QWidget *, QUuid> _pages_uuids_map;

  OilFieldWidget *_oilFieldWidget;

  FilteringTableAssistant *_eventsTable;
};

} // namespace Main
