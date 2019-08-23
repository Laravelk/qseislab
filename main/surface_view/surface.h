#pragma once

#include <Q3DSurface>
#include <QtDataVisualization>
#include <QtWidgets>
#include <map>
#include <memory>
#include <vector>

using namespace QtDataVisualization;

typedef QUuid Uuid;
typedef std::tuple<float, float, float> Point;

namespace Data {
class SeismEvent;
class SeismHorizon;
class SeismProject;
class SeismPoint;
class SeismReciever;
} // namespace Data

namespace Main {
class Surface : public QObject {
  Q_OBJECT
public:
  explicit Surface(Q3DSurface *surface);
  ~Surface() = default;

  void addEvent(const std::unique_ptr<Data::SeismEvent> &event);
  void addHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon);
  void addReciever(const std::unique_ptr<Data::SeismReciever> &reciever);

  bool showEvent(QUuid uid);
  bool showEvent(std::unique_ptr<Data::SeismEvent> &event);

  void setProject(const std::unique_ptr<Data::SeismProject> &project);

  bool removeEvent(const std::unique_ptr<Data::SeismEvent> &event);
  bool removeEvent(const Uuid uid);
  bool removeHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon);
  bool removeHorizon(const Uuid uid);
  bool removeReciever(const std::unique_ptr<Data::SeismReciever> &reciever);
  bool removeReciever(const Uuid uid);

  bool hideEvent(QUuid uid);
  bool hideEvent(std::unique_ptr<Data::SeismEvent> &event);

  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMoveEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

  void setFiltr(bool (*func)(float, float));

  const std::map<Uuid, QCustom3DItem *> getEventMap() const;
  const std::map<Uuid, QSurface3DSeries *> getHorizonMap() const;

private:
  Q3DSurface *_surface;
  QCustom3DLabel *_label;
  Data::SeismProject *_project;
  QImage _blackColor;
  QImage _redColor;
  bool _isHandle;
  QCustom3DItem *_itemHandle;
  QColor _colours[10] = {QColor(255, 0, 0),     QColor(0, 255, 0),
                         QColor(0, 0, 255),     QColor(0, 0, 0),
                         QColor(255, 255, 255), QColor(0, 128, 64),
                         QColor(128, 128, 128)};
  int _indexColor = 0;

private:
  void handleElementSelected(QAbstract3DGraph::ElementType type);
  void handleElementDoubleClicked(QAbstract3DGraph::ElementType type);

  std::map<Uuid, QCustom3DItem *> _eventMap;
  std::map<Uuid, QSurface3DSeries *> _horizonMap;
  std::map<Uuid, QCustom3DItem *> _recieverMap;
  std::vector<QSurfaceDataRow *> _rowVector;
  std::vector<Point> _pointVector;
};
} // namespace Main
