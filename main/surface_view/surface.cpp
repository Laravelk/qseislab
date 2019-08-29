#include "surface.h"

#include "../../data/seismevent.h"
#include "../../data/seismhorizon.h"
#include "../../data/seismproject.h"
#include "../../data/seismwell.h"

#include <iostream>

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismProject SeismProject;
typedef Data::SeismWell SeismWell;

namespace Main {
Surface::Surface(Q3DSurface *surface) : _surface(surface), _isHandle(false) {
  _blackColor = QImage(2, 2, QImage::Format_RGB32);
  _redColor = QImage(2, 2, QImage::Format_RGB32);
  _blackColor.fill(Qt::black);
  _redColor.fill(Qt::red);

  _surface->setAxisX(new QValue3DAxis);
  _surface->setAxisY(new QValue3DAxis);
  _surface->setAxisZ(new QValue3DAxis);

  _surface->scene()->activeCamera()->setCameraPreset(
      Q3DCamera::CameraPresetIsometricLeftHigh);
  surface->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);

  _surface->axisX()->setTitle("X, meters");
  _surface->axisY()->setTitle("Z, meters");
  _surface->axisZ()->setTitle("Y, meters");

  _surface->axisX()->setRange(-100, 0);
  _surface->axisY()->setRange(-100, 0);
  _surface->axisZ()->setRange(-100, 0);

  _surface->axisX()->setTitleVisible(true);
  _surface->axisY()->setTitleVisible(true);
  _surface->axisZ()->setTitleVisible(true);

  connect(_surface, &QAbstract3DGraph::selectedElementChanged, this,
          &Surface::handleElementSelected);
}

void Surface::addEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  Point eventPoint = event->getLocation();
  float x, y, z;
  std::tie(x, y, z) = eventPoint;
  QVector3D position(x, z, y);
  QCustom3DItem *item = new QCustom3DItem(":/sphereSmooth.obj", position,
                                          QVector3D(0.035f, 0.035f, 0.035f),
                                          QQuaternion(), _blackColor);
  item->setShadowCasting(false);
  item->setVisible(event->isProcessed());
  _surface->addCustomItem(item);
  _eventMap.insert(std::pair<Uuid, QCustom3DItem *>(event->getUuid(), item));
}

void Surface::addHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon) {
  _pointVector = horizon->getPoints();
  unsigned long countElementInLine =
      static_cast<unsigned long>(horizon->getNx());
  unsigned long countElementInColumn =
      static_cast<unsigned long>(horizon->getNy());
  QSurfaceDataArray *dataArray = new QSurfaceDataArray;
  for (unsigned long i = 0; i < countElementInLine; i++) {
    _rowVector.push_back(new QSurfaceDataRow);
  }
  for (unsigned long i = 0; i < countElementInLine; i++) {
    for (unsigned long j = 0; j < countElementInColumn; j++) {
      float x = 0, y = 0, z = 0;
      std::tie(x, y, z) = _pointVector.at(countElementInLine * i + j);
      *_rowVector.at(i) << QVector3D(x, z, y);
    }
  }
  for (unsigned long i = 0; i < countElementInLine; i++) {
    *dataArray << _rowVector.at(i);
  }

  QSurface3DSeries *series = new QSurface3DSeries;
  series->dataProxy()->resetArray(dataArray);
  series->setDrawMode(QSurface3DSeries::DrawSurface);
  series->setBaseColor(_colours[_indexColor++ % 7]);
  _surface->addSeries(series);
  _horizonMap.insert(
      std::pair<Uuid, QSurface3DSeries *>(horizon->getUuid(), series));
  _rowVector.clear();
}

void Surface::addReceiver(
    const std::unique_ptr<Data::SeismReceiver> &receiver) {
  Point receiverPoint = receiver->getLocation();
  float x, y, z;
  std::tie(x, y, z) = receiverPoint;
  QVector3D position(x, z, y);
  QCustom3DItem *item = new QCustom3DItem(":/minimalSmooth.obj", position,
                                          QVector3D(0.035f, 0.035f, 0.035f),
                                          QQuaternion(), _redColor);
  item->setShadowCasting(false);
  _surface->addCustomItem(item);
  _receiverMap.insert(
      std::pair<Uuid, QCustom3DItem *>(receiver->getUuid(), item));
}

void Surface::addWell(const std::unique_ptr<Data::SeismWell> &well) {
  Point lastPoint = well->getPoint(0);
  for (auto &point : well->getPoints()) {
    float x = 0, y = 0, z = 0;
    std::tie(x, y, z) = point;
    std::cerr << x << " " << y << " " << z << std::endl;
    QVector3D rotateVector = vectorBy2Point(lastPoint, point);
    QVector3D position(x, y, z);
    lastPoint = point;
    QCustom3DItem *item = new QCustom3DItem(
        ":/cylinderFilledSmooth.obj", position,
        QVector3D(0.035f, 0.035f, 0.035f), QQuaternion(), _blackColor);
    item->setVisible(true);
    _surface->addCustomItem(item);
  }
}

bool Surface::showEvent(QUuid uid) {
  if (_eventMap.at(uid)) {
    _eventMap.at(uid)->setVisible(true);
    return true;
  }
  return false;
}

bool Surface::showEvent(std::unique_ptr<Data::SeismEvent> &event) {
  if (_eventMap.at(event->getUuid())) {
    _eventMap.at(event->getUuid())->setVisible(true);
    return true;
  }
  return false;
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
  if (item == _itemHandle) {
    _isHandle = false;
    _surface->removeCustomItemAt(_label->position());
  }
  QVector3D position = item->position();
  if (_eventMap.erase(uid)) {
    _surface->removeCustomItemAt(position);
    return true;
  }
  return false;
}

bool Surface::removeHorizon(
    const std::unique_ptr<Data::SeismHorizon> &horizon) {
  return removeHorizon(horizon.get()->getUuid());
}

bool Surface::removeHorizon(const Uuid uid) {
  QSurface3DSeries *series = _horizonMap[uid];
  if (_horizonMap.erase(uid)) {
    _surface->removeSeries(series);
    return true;
  }
  return false;
}

bool Surface::removeReceiver(
    const std::unique_ptr<Data::SeismReceiver> &receiver) {
  return removeReceiver(receiver.get()->getUuid());
}

bool Surface::removeReceiver(const Uuid uid) {
  QCustom3DItem *item = _receiverMap[uid];
  if (_receiverMap.erase(uid)) {
    _surface->removeCustomItem(item);
    return true;
  }
  return false;
}

bool Surface::removeWell(const std::unique_ptr<Data::SeismWell> &well) {
  return removeWell(well.get()->getUuid());
}

bool Surface::removeWell(const Uuid uid) {
  QSurface3DSeries *series = _wellMap[uid];
  if (_wellMap.erase(uid)) {
    _surface->removeSeries(series);
    return true;
  }
  return false;
}

bool Surface::hideEvent(QUuid uid) {
  if (_eventMap.at(uid)) {
    _eventMap.at(uid)->setVisible(true);
  }
  return false;
}

bool Surface::hideEvent(std::unique_ptr<Data::SeismEvent> &event) {
  if (_eventMap.at(event->getUuid())) {
    _eventMap.at(event->getUuid())->setVisible(true);
    return true;
  }
  return false;
}

const std::map<Uuid, QCustom3DItem *> Surface::getEventMap() const {
  return _eventMap;
}

const std::map<Uuid, QSurface3DSeries *> Surface::getHorizonMap() const {
  return _horizonMap;
}

void Surface::handleElementSelected(QAbstract3DGraph::ElementType type) {
  if (type == QAbstract3DGraph::ElementCustomItem) {
    QCustom3DItem *item = _surface->selectedCustomItem();
    _itemHandle = item;
    if (_isHandle) {
      _surface->removeCustomItemAt(_label->position());
    }
    QVector3D sizeOfLabel(1.0f, 1.0f, 0.0f);
    QVector3D positionOfLabel;
    positionOfLabel.setX(
        static_cast<float>(static_cast<double>(item->position().x()) + 0.1));
    positionOfLabel.setY(
        static_cast<float>(static_cast<double>(item->position().y()) + 0.1));
    positionOfLabel.setZ(
        static_cast<float>(static_cast<double>(item->position().z()) + 0.1));
    _label = new QCustom3DLabel(
        "X:" +
            QString::number(static_cast<double>(item->position().x()), 'g', 3) +
            " Y:" +
            QString::number(static_cast<double>(item->position().z()), 'g', 3) +
            " Z:" +
            QString::number(static_cast<double>(item->position().y()), 'g', 3),
        QFont("Century Gothic", 30), positionOfLabel, sizeOfLabel,
        QQuaternion());
    _label->setFacingCamera(true);
    _surface->addCustomItem(_label);
    _isHandle = true;
    return;
  }
  _surface->clearSelection();
}

QVector3D Surface::vectorBy2Point(Point pointOne, Point pointTwo) {
  float x1 = 0, y1 = 0, z1 = 0;
  float x2 = 0, y2 = 0, z2 = 0;
  std::tie(x1, y1, z1) = pointOne;
  std::tie(x2, y2, z2) = pointTwo;
  return QVector3D(x2 - x1, y2 - y1, z2 - z1);
}

} // namespace Main
