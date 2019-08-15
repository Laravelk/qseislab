#pragma once

#include <Q3DSurface>
#include <QtDataVisualization>
#include <QtWidgets>
#include <map>
#include <memory>
#include <vector>

using namespace QtDataVisualization;

typedef QUuid Uuid;

namespace Data {
class SeismEvent;
class SeismHorizon;
class SeismProject;
class SeismPoint;
} // namespace Data

namespace Main {
class Surface : public QObject {
  Q_OBJECT
public:
  explicit Surface(Q3DSurface *surface);
  ~Surface();

  void addEvent(const std::unique_ptr<Data::SeismEvent> &event);
  void addHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon);

  void setProject(const std::unique_ptr<Data::SeismProject> &project);

  bool removeEvent(const std::unique_ptr<Data::SeismEvent> &event);
  bool removeEvent(const Uuid uid);
  bool removeHorizon(const std::unique_ptr<Data::SeismEvent> &event);
  bool removeHorizon(const Uuid uid);

  const std::map<Uuid, QCustom3DItem *> getEventMap() const;
  const std::map<Uuid, QSurface3DSeries *> getHorizonMap() const;

private:
  Q3DSurface *_surface;
  QCustom3DLabel *_label;
  Data::SeismProject *_project;
  QImage _blackColor;
  QImage _redColor;
  bool _isHandle;
  float a = 0.0f; // TODO delete

private:
  void addEventInGraph(const std::unique_ptr<Data::SeismEvent> &event);
  void addHorizonInGraph(const std::unique_ptr<Data::SeismHorizon> &horizon);
  void handleElementSelected(QAbstract3DGraph::ElementType type);

  std::map<Uuid, QCustom3DItem *> _eventMap;
  std::map<Uuid, QSurface3DSeries *> _horizonMap;
};
} // namespace Main
