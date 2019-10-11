#include "surface.h"
#include "../../data/seismevent.h"
#include "../../data/seismhorizon.h"
#include "../../data/seismproject.h"
#include "../../data/seismwell.h"

#include <iostream> // TODO: delete

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
  settingGraph();
  surface->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
}

void Surface::addEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  float x, y, z;
  std::tie(x, y, z) = event->getLocation();
  QVector3D position(x, z, y);
  QCustom3DItem *item = new QCustom3DItem(
      ":/sphereSmooth.obj", position,
      QVector3D(SCALING_SPHERE, SCALING_SPHERE, SCALING_SPHERE), QQuaternion(),
      _blackColor);
  item->setShadowCasting(false);
  item->setVisible(event->isProcessed());
  _surface->addCustomItem(item);
  checkAxisRange(*item);
  _events.insert(std::pair<Uuid, QCustom3DItem *>(event->getUuid(), item));
}

void Surface::addHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon) {
  _points = horizon->getPoints();
  unsigned long countElementInLine =
      static_cast<unsigned long>(horizon->getNx());
  unsigned long countElementInColumn =
      static_cast<unsigned long>(horizon->getNy());
  QSurfaceDataArray *dataArray = new QSurfaceDataArray;
  fillSurfaceDataRow(dataArray, countElementInLine, countElementInColumn);
  QSurface3DSeries *series = createHorizonSeries(dataArray);
  _surface->addSeries(series);
  _horizons.insert(
      std::pair<Uuid, QSurface3DSeries *>(horizon->getUuid(), series));
  _rows.clear();
}

void Surface::addReceiver(
    const std::unique_ptr<Data::SeismReceiver> &receiver) {
  float x, y, z;
  std::tie(x, y, z) = receiver->getLocation();
  QVector3D position(x, z, y);
  QCustom3DItem *item = new QCustom3DItem(
      ":/minimalSmooth.obj", position,
      QVector3D(SCALING_SMOOTH, SCALING_SMOOTH, SCALING_SMOOTH), QQuaternion(),
      _redColor);
  item->setShadowCasting(false);
  item->setVisible(true);
  _surface->addCustomItem(item);
  checkAxisRange(*item);
  _receivers.insert(
      std::pair<Uuid, QCustom3DItem *>(receiver->getUuid(), item));
}

void Surface::addWell(const std::unique_ptr<Data::SeismWell> &well) {
  float x = 0, y = 0, z = 0;
  float lx = 0, ly = 0, lz = 0;
  std::tie(lx, ly, lz) = well->getPoint(0);
  for (auto point = ++begin(well->getPoints()); point != end(well->getPoints());
       ++point) {
    std::tie(x, y, z) = *point;
    QVector3D pipeVector =
        vectorBy2Point(QVector3D(lx, ly, lz), QVector3D(x, y, z));
    float scaling = calculateLenght(QVector3D(x, y, z), QVector3D(lx, ly, lz)) /
                    _maxAxisValue / 1.4;
    std::tie(lx, ly, lz) = *point;
    QCustom3DItem *item = new QCustom3DItem(
        ":/cylinderFilledSmooth.obj",
        QVector3D((x + lx) / 2, (z + lz) / 2, (y + ly) / 2),
        QVector3D(SCALING_OX, scaling, SCALING_OY),
        QQuaternion::rotationTo(QVector3D(0.0f, 0.0f, 1.0f), pipeVector),
        _blackColor);
    checkAxisRange(*item);
    item->setScalingAbsolute(true);
    item->setVisible(true);
    _wells[well->getUuid()].push_back(item);
    _surface->addCustomItem(item);
  }
}

bool Surface::showEvent(QUuid uid) {
  if (_events.at(uid)) {
    _events.at(uid)->setVisible(true);
    return true;
  }
  return false;
}

bool Surface::showEvent(std::unique_ptr<Data::SeismEvent> &event) {
  if (_events.at(event->getUuid())) {
    _events.at(event->getUuid())->setVisible(true);
    return true;
  }
  return false;
}

void Surface::setProject(const std::unique_ptr<Data::SeismProject> &project) {
  for (auto &uuid_event : project->getAllMap<SeismEvent>()) {
    addEvent(uuid_event.second);
    showEvent(uuid_event.first);
  }
  for (auto &uuid_horizon : project->getAllMap<SeismHorizon>()) {
    addHorizon(uuid_horizon.second);
  }
  for (auto &uuid_well : project->getAllMap<SeismWell>()) {
    addWell(uuid_well.second);
    for (auto &receiver : uuid_well.second->getReceivers()) {
      addReceiver(receiver);
    }
  }
}

bool Surface::removeEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  Uuid uid = event.get()->getUuid();
  return removeEvent(uid);
}

bool Surface::removeEvent(const Uuid uid) {
  QCustom3DItem *item = _events[uid];
  if (item == _isItemHanlde) {
    _isHandle = false;
    _surface->removeCustomItemAt(_label->position());
  }
  QVector3D position = item->position();
  if (_events.erase(uid)) {
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
  QSurface3DSeries *series = _horizons[uid];
  if (_horizons.erase(uid)) {
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
  QCustom3DItem *item = _receivers[uid];
  if (_receivers.erase(uid)) {
    _surface->removeCustomItem(item);
    return true;
  }
  return false;
}

bool Surface::removeWell(const std::unique_ptr<Data::SeismWell> &well) {
  return removeWell(well.get()->getUuid());
}

bool Surface::removeWell(const Uuid uid) {
  std::vector<QCustom3DItem *> v = _wells[uid];
  if (_wells.erase(uid)) {
    for (auto &it : v) {
      _surface->removeCustomItem(it);
    }
    return true;
  }
  return false;
}

bool Surface::hideEvent(QUuid uid) {
  if (_events.at(uid)) {
    _events.at(uid)->setVisible(true);
  }
  return false;
}

bool Surface::hideEvent(std::unique_ptr<Data::SeismEvent> &event) {
  if (_events.at(event->getUuid())) {
    _events.at(event->getUuid())->setVisible(true);
    return true;
  }
  return false;
}

const std::map<Uuid, QCustom3DItem *> Surface::getEvents() const {
  return _events;
}

const std::map<Uuid, QSurface3DSeries *> Surface::getHorizons() const {
  return _horizons;
}

void Surface::handleElementSelected(QAbstract3DGraph::ElementType type) {
  if (type == QAbstract3DGraph::ElementCustomItem) {
    QCustom3DItem *item = _surface->selectedCustomItem();
    _isItemHanlde = item;
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
        QFont("Century Gothic", FONT_SIZE), positionOfLabel, sizeOfLabel,
        QQuaternion());
    _label->setFacingCamera(true);
    _surface->addCustomItem(_label);
    _isHandle = true;
    return;
  }
  _surface->clearSelection();
}

void Surface::settingGraph() {

  _surface->scene()->activeCamera()->setCameraPreset(
      Q3DCamera::CameraPresetIsometricLeftHigh);

  // setting axis
  _surface->setAxisX(new QValue3DAxis);
  _surface->setAxisY(new QValue3DAxis);
  _surface->setAxisZ(new QValue3DAxis);

  _surface->axisX()->setTitle("X, meters");
  _surface->axisY()->setTitle("Z, meters");
  _surface->axisZ()->setTitle("Y, meters");

  _surface->axisX()->setAutoAdjustRange(true);
  _surface->axisY()->setAutoAdjustRange(true);
  _surface->axisZ()->setAutoAdjustRange(true);

  _minAxisValue = -DEFAULT_AXIS_MODULE_RANGE;
  _maxAxisValue = DEFAULT_AXIS_MODULE_RANGE;

  _surface->axisX()->setRange(_minAxisValue, _maxAxisValue);
  _surface->axisY()->setRange(_minAxisValue, _maxAxisValue);
  _surface->axisZ()->setRange(_minAxisValue, _maxAxisValue);

  _surface->axisX()->setTitleVisible(true);
  _surface->axisY()->setTitleVisible(true);
  _surface->axisZ()->setTitleVisible(true);
  // end setting axis

  connect(_surface, &QAbstract3DGraph::selectedElementChanged, this,
          &Surface::handleElementSelected);
}

void Surface::checkAxisRange(QCustom3DItem &newItem) {
  QVector3D position = newItem.position();
  //  std::cerr << position.x() << " " << position.y() << " " << position.z()
  //            << std::endl;
  if (position.x() < _minAxisValue | position.y() < _minAxisValue |
      position.z() < _minAxisValue) {
    float minValue =
        std::min(std::min(position.x(), position.y()), position.z());
    _minAxisValue = minValue;
  }

  if (position.x() > _maxAxisValue | position.y() > _maxAxisValue |
      position.z() > _maxAxisValue) {
    float maxValue =
        std::max(std::max(position.x(), position.y()), position.z());
    _maxAxisValue = maxValue;
  }

  _surface->axisX()->setRange(_minAxisValue, _maxAxisValue);
  _surface->axisY()->setRange(_minAxisValue, _maxAxisValue);
  _surface->axisZ()->setRange(_minAxisValue, _maxAxisValue);
}

void Surface::fillSurfaceDataRow(QSurfaceDataArray *dataRow,
                                 ulong countElementInLine,
                                 ulong countElementInColumn) {
  for (unsigned long i = 0; i < countElementInLine; i++) {
    _rows.push_back(new QSurfaceDataRow);
  }
  for (unsigned long i = 0; i < countElementInLine; i++) {
    for (unsigned long j = 0; j < countElementInColumn; j++) {
      float x = 0, y = 0, z = 0;
      std::tie(x, y, z) = _points.at(countElementInLine * i + j);
      *_rows.at(i) << QVector3D(x, z, y);
    }
  }
  for (unsigned long i = 0; i < countElementInLine; i++) {
    *dataRow << _rows.at(i);
  }
}

QSurface3DSeries *Surface::createHorizonSeries(QSurfaceDataArray *dataArray) {
  QSurface3DSeries *series = new QSurface3DSeries;
  series->dataProxy()->resetArray(dataArray);
  series->setDrawMode(QSurface3DSeries::DrawSurface);
  series->setBaseColor(_colours[_indexColor++ % 7]);
  return series;
}

QVector3D Surface::vectorBy2Point(QVector3D from, QVector3D to) {
  float x1 = from.x(), y1 = from.y(), z1 = from.z();
  float x2 = to.x(), y2 = to.y(), z2 = to.z();
  return QVector3D(x2 - x1, y2 - y1, z2 - z1);
}

float Surface::calculateLenght(QVector3D from, QVector3D to) {
  return static_cast<float>(sqrt((from.x() - to.x()) * (from.x() - to.x()) +
                                 (from.y() - to.y()) * (from.y() - to.y()) +
                                 (from.z() - to.z()) * (from.z() - to.z())));
}

} // namespace Main
