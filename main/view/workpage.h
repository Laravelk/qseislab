#pragma once

#include "data/seismproject.h"

#include "tableassistant.h" // TODO: definition-class
#include <QFrame>
#include <QTableWidget>
#include <QtDataVisualization>

#include <memory>

using namespace QtDataVisualization;

namespace Main {
class InfoProject;
class Surface;
class WorkPage : public QFrame {
  Q_OBJECT

public:
  explicit WorkPage(QWidget *parent = nullptr);

  void loadProject(const std::unique_ptr<Data::SeismProject> &);

  void updateProject(const std::unique_ptr<Data::SeismEvent> &);
  void
  updateProject(const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &);
  void updateProjectRemoveEvent(const QUuid &);

  void updateProject(const std::unique_ptr<Data::SeismHorizon> &);
  void updateProjectRemoveHorizon(const QUuid &);

  void updateProject(const std::unique_ptr<Data::SeismWell> &);
  void updateProjectRemoveWell(const QUuid &);

signals:
  void viewEventClicked(const QUuid) const;
  void removeEventClicked(const QUuid) const;

private slots:
  //  void handleEventClicked(int, int);

private:
  //  void clearTable();
  //  void initEventsTable(QTableWidget *);

  //  void insertEventInTable(const std::unique_ptr<Data::SeismEvent> &);
  //  void removeEventInTable(const QUuid &);

  InfoProject *_infoProject;
  TableAssistant *_eventsTable;
  //  QTableWidget *_eventsTable;
  Surface *_surface;
  Q3DSurface *_graph;
};

} // namespace Main
