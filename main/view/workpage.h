#pragma once

#include "data/seismproject.h"

//#include <QCheckBox>
#include <QFrame>
#include <QTableWidget>
//#include <QtDataVisualization>

#include <memory>

//using namespace QtDataVisualization;

class FilteringTableAssistant;

namespace Main {
class InfoProject;
//class OilFieldScene;
class OilFieldWidget;
class WorkPage : public QFrame {
  Q_OBJECT

public:
  explicit WorkPage(QWidget *parent = nullptr);

  void loadProject(Data::SeismProject const * const);

  const QUuid getFocusEvent() const;

  void addEvent(Data::SeismEvent const * const);
  void
  processedEvents(const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &);
  void updateEvent(Data::SeismEvent const * const);
  void removeEvent(const QUuid &);

  void addHorizon(Data::SeismHorizon const * const);
  void removeHorizon(const QUuid &);

  void addWell(Data::SeismWell const * const);
  void removeWell(const QUuid &);

  void
  addReceiver(Data::SeismReceiver const * const); // TODO: re-see
  void removeReceiver(const QUuid &);                        // TODO: re-see

signals:
  void eventSelectionChanged(const std::set<QUuid> &) const;

  void viewEventClicked(const QUuid &) const;
  void removeEventClicked(const QUuid &) const;

private:
    QTabWidget* _workPages;
    OilFieldWidget* _oilFieldWidget;

  FilteringTableAssistant *_eventsTable;

//  OilFieldScene *_oilFieldScene;

//  Q3DSurface *_graph;
//  QCheckBox *_eventBox;
//  QCheckBox *_receiverBox;
//  QCheckBox *_wellBox;
//  QCheckBox *_horizonBox;
};

} // namespace Main
