#pragma once

#include <Q3DSurface>
#include <QtDataVisualization>
#include <QtWidgets>
#include <memory>
#include <vector>

using namespace QtDataVisualization;

namespace Data {
class SeismEvent;
class SeismHorizon;
class SeismProject;
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

private:
  Q3DSurface *_surface;
  QCustom3DItem *_ball;
  QCustom3DItem *_oval;
  QVector3D *_previousScaling;

private:
  void addEventInGraph(const std::unique_ptr<Data::SeismEvent> &event);
  void addHorizonInGraph();
};
} // namespace Main
