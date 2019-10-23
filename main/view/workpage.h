#pragma once

#include "data/seismproject.h"

#include <QCheckBox>
#include <QFrame>
#include <QTableWidget>
#include <QtDataVisualization>

#include <memory>

using namespace QtDataVisualization;

class TableAssistant;

namespace Main {
class InfoProject;
class OilFieldScene;
class WorkPage : public QFrame {
  Q_OBJECT

public:
  explicit WorkPage(QWidget *parent = nullptr);

  void loadProject(const std::unique_ptr<Data::SeismProject> &);

  void addEvent(const std::unique_ptr<Data::SeismEvent> &);
  void
  processedEvents(const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &);
  void updateEvent(const std::unique_ptr<Data::SeismEvent> &);
  void removeEvent(const QUuid &);

  void addHorizon(const std::unique_ptr<Data::SeismHorizon> &);
  void removeHorizon(const QUuid &);

  void addWell(const std::unique_ptr<Data::SeismWell> &);
  void removeWell(const QUuid &);

  void
  addReceiver(const std::unique_ptr<Data::SeismReceiver> &); // TODO: re-see
  void removeReceiver(const QUuid &);                        // TODO: re-see

signals:
  void viewEventClicked(const QUuid) const;
  void removeEventClicked(const QUuid) const;

private:
  TableAssistant *_eventsTable;
  OilFieldScene *_oilFieldScene;
  Q3DSurface *_graph;
  QCheckBox *_eventBox;
  QCheckBox *_receiverBox;
  QCheckBox *_wellBox;
  QCheckBox *_horizonBox;
};

} // namespace Main
