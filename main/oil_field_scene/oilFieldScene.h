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
class SeismReceiver;
class SeismWell;
} // namespace Data

namespace Main {
class OilFieldScene : public QObject {
  Q_OBJECT
public:
  explicit OilFieldScene(Q3DSurface *);
  ~OilFieldScene() = default;

  void addEvent(const std::shared_ptr<Data::SeismEvent> &);
  void addHorizon(const std::shared_ptr<Data::SeismHorizon> &);
  void addReceiver(const std::shared_ptr<Data::SeismReceiver> &);
  void addWell(const std::shared_ptr<Data::SeismWell> &);

  bool showEvent(const QUuid &);
  //  bool showEvent(const std::unique_ptr<Data::SeismEvent> &);

  void setProject(const std::unique_ptr<Data::SeismProject> &);

  //  bool removeEvent(const std::unique_ptr<Data::SeismEvent> &);
  bool removeEvent(const Uuid &);
  //  bool removeHorizon(const std::unique_ptr<Data::SeismHorizon> &);
  bool removeHorizon(const Uuid &);
  //  bool removeReceiver(const std::unique_ptr<Data::SeismReceiver> &);
  bool removeReceiver(const Uuid &);
  //  bool removeWell(const std::unique_ptr<Data::SeismWell> &);
  bool removeWell(const Uuid &);

  bool hideEvent(const QUuid &);
  //  bool hideEvent(const std::unique_ptr<Data::SeismEvent> &);
  void hideAllEvent(bool);
  void hideAllWell(bool);
  void hideAllReceiver(bool);
  void hideAllHorizon(bool);

  void mouseDoubleClickEvent(QMouseEvent *);
  void mouseMoveEvent(QMoveEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);

  void setFiltr(bool (*func)(float, float)); // TODO: filter

  const std::map<Uuid, QCustom3DItem *> getEvents() const;
  const std::map<Uuid, QSurface3DSeries *> getHorizons() const;

  bool isEventsHide() const { return _isEventsHide; }
  bool isWellsHide() const { return _isWellsHide; }
  bool isReceiversHide() const { return _isReceiversHide; }
  bool isHorizonsHide() const { return _isHorizonsHide; }

private:
  Q3DSurface *_surface;
  QCustom3DLabel *_label;
  QImage _blackColor;
  QImage _redColor;
  bool _isHandle;
  QCustom3DItem *_isItemHanlde;
  QColor _colours[10] = {QColor(255, 0, 0, 100),     QColor(0, 255, 0, 100),
                         QColor(0, 0, 255, 100),     QColor(0, 0, 0, 100),
                         QColor(255, 255, 255, 100), QColor(0, 128, 64, 100),
                         QColor(128, 128, 128, 100)};
  int _indexColor = 0;
  float _minAxisValue;
  float _maxAxisValue;
  bool _isEventsHide = false;
  bool _isWellsHide = false;
  bool _isReceiversHide = false;
  bool _isHorizonsHide = false;

private:
  void removeItemHandle();
  void handleElementSelected(QAbstract3DGraph::ElementType);
  void handleElementDoubleClicked(QAbstract3DGraph::ElementType);
  void settingGraph();
  void checkAxisRange(QCustom3DItem &);
  void checkAxisRange(QVector3D &position);
  void fillSurfaceDataRow(QSurfaceDataArray *, ulong, ulong);
  QSurface3DSeries *createHorizonSeries(QSurfaceDataArray *);
  QVector3D vectorBy2Point(QVector3D, QVector3D);
  float calculateLenght(QVector3D, QVector3D);

  std::map<Uuid, QCustom3DItem *> _events;
  std::map<Uuid, QSurface3DSeries *> _horizons;
  std::map<Uuid, QCustom3DItem *> _receivers;
  std::map<Uuid, std::vector<QCustom3DItem *>> _wells;
  std::vector<QSurfaceDataRow *> _rows;
  std::vector<Point> _points;

  const float SCALING_OX = 0.006f;
  const float SCALING_OY = 0.006f;
  const float SCALING_SPHERE = 0.009f;
  const float SCALING_SMOOTH = 0.009f;
  const float DEFAULT_AXIS_MODULE_RANGE = 1000;
  const int FONT_SIZE = 30;
};
} // namespace Main
