#include "surface.h"

#include "../../data/seismevent.h"
#include "../../data/seismhorizon.h"
#include "../../data/seismproject.h"

#include <iostream>

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismProject SeismProject;

namespace Main {
Surface::Surface(Q3DSurface *surface) : _surface(surface), _isHandle(false) {
  _blackColor = QImage(2, 2, QImage::Format_RGB32);
  _redColor = QImage(2, 2, QImage::Format_RGB32);
  _blackColor.fill(Qt::black);
  _redColor.fill(Qt::red);

  _surface->setAxisX(new QValue3DAxis);
  _surface->setAxisY(new QValue3DAxis);
  _surface->setAxisZ(new QValue3DAxis);

  _surface->axisX()->setRange(-100.0f, 100.0f);
  _surface->axisZ()->setRange(-100.0f, 100.0f);
  _surface->axisY()->setRange(-100.0f, 100.0f);

  _surface->axisX()->setTitle("X, meters");
  _surface->axisY()->setTitle("Y, meters");
  _surface->axisZ()->setTitle("Z, meters");

  _surface->axisX()->setTitleVisible(true);
  _surface->axisY()->setTitleVisible(true);
  _surface->axisZ()->setTitleVisible(true);

  _surface->axisX()->setAutoAdjustRange(false);
  _surface->axisY()->setAutoAdjustRange(false);
  _surface->axisZ()->setAutoAdjustRange(false);

  //  _surface->reportContentOrientationChange(Qt::InvertedLandscapeOrientation);
  connect(_surface, &QAbstract3DGraph::selectedElementChanged, this,
          &Surface::handleElementSelected);
}

Surface::~Surface() {
  _surface->removeCustomItems();
  delete _surface;
}

void Surface::addEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  addEventInGraph(event);
}

// read from horizon after interpolation
void Surface::addHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon) {
  _pointVector = horizon->getPoints();
  unsigned long countElementInLine = 200;
  unsigned long countElementInColumn = 200;
  _dataArray = new QSurfaceDataArray;
  for (int i = 0; i < countElementInLine; i++) {
    _rowVector.push_back(new QSurfaceDataRow);
  }
  for (unsigned long i = 0; i < countElementInLine; i++) {
    for (unsigned long j = 0; j < countElementInColumn; j++) {
      float x = 0, y = 0, z = 0;
      std::tie(x, y, z) = _pointVector.at(countElementInLine * i + j);
      *_rowVector.at(i) << QVector3D(x, y, z);
    }
  }
  for (unsigned long i = 0; i < countElementInLine; i++) {
    *_dataArray << _rowVector.at(i);
  }

  QSurface3DSeries *series = new QSurface3DSeries;
  series->dataProxy()->resetArray(_dataArray);
  _surface->addSeries(series);
}

void Surface::setProject(const std::unique_ptr<Data::SeismProject> &project) {
  _project = project.get();
}

bool Surface::removeEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  Uuid uid = event.get()->getUuid();
  return removeEvent(uid);
}

bool Surface::removeEvent(const Uuid uid) {
  QCustom3DItem *item = _eventMap[uid];
  QVector3D position = item->position();
  if (_eventMap.erase(uid)) {
    _surface->removeCustomItemAt(position);
    return true;
  }
  return false;
}

bool Surface::removeHorizon(const std::unique_ptr<Data::SeismEvent> &event) {}

const std::map<Uuid, QCustom3DItem *> Surface::getEventMap() const {
  return _eventMap;
}

const std::map<Uuid, QSurfaceDataArray *> Surface::getHorizonMap() const {
  return _horizonMap;
}

void Surface::addEventInGraph(const std::unique_ptr<Data::SeismEvent> &event) {
  QVector3D position(a, 0.0f, 0.0f); // TODO read from event
  a += 0.1f;
  QCustom3DItem *item = new QCustom3DItem(
      "/Users/ivanmorozov/MyQtProject/Геология/qseislab/resources/"
      "sphereSmooth.obj",
      position, QVector3D(0.025f, 0.025f, 0.025f), QQuaternion(), _blackColor);
  item->setShadowCasting(false);
  _surface->addCustomItem(item);
  _eventMap.insert(std::pair<Uuid, QCustom3DItem *>(event->getUuid(), item));
}

void Surface::handleElementSelected(QAbstract3DGraph::ElementType type) {
  if (type == QAbstract3DGraph::ElementCustomItem) {
    QCustom3DItem *item = _surface->selectedCustomItem();
    if (_isHandle) {
      _surface->removeCustomItemAt(_label->position());
    }
    QVector3D sizeOfLabel(1.0f, 1.0f, 0.0f);
    QVector3D positionOfLabel;
    positionOfLabel.setX(item->position().x() + 0.1);
    positionOfLabel.setY(item->position().y() + 0.1);
    positionOfLabel.setZ(item->position().z() + 0.1);
    _label = new QCustom3DLabel(
        "X:" + QString::number(item->position().x(), 'g', 3) +
            " Y:" + QString::number(item->position().y(), 'g', 3) +
            " Z:" + QString::number(item->position().z(), 'g', 3),
        QFont("Century Gothic", 30), positionOfLabel, sizeOfLabel,
        QQuaternion());
    _label->setFacingCamera(true);
    _surface->addCustomItem(_label);
    _isHandle = true;
  }
}

} // namespace Main
