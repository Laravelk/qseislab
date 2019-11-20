#include "oilFieldScene.h"
#include "data/seismevent.h"
#include "data/seismhorizon.h"
#include "data/seismproject.h"
#include "data/seismwell.h"

#include <assert.h>

//#include <iostream> // TODO: delete

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismProject SeismProject;
typedef Data::SeismWell SeismWell;

namespace Main {
OilFieldScene::OilFieldScene(Q3DSurface *surface)
    : _surface(surface), _isHandle(false) {
  _blackColor = QImage(2, 2, QImage::Format_RGB32);
  _redColor = QImage(2, 2, QImage::Format_RGB32);
  _blackColor.fill(Qt::black);
  _redColor.fill(Qt::red);
  settingGraph();
  surface->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
  surface->axisY()->setReversed(true);
}

void OilFieldScene::addEvent(const std::shared_ptr<Data::SeismEvent> &event) {
  //  if (event->isProcessed()) {
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
  //  }
}

void OilFieldScene::addHorizon(
    const std::shared_ptr<Data::SeismHorizon> &horizon) {
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

void OilFieldScene::addReceiver(
    const std::shared_ptr<Data::SeismReceiver> &receiver) {
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

void OilFieldScene::addWell(const std::shared_ptr<Data::SeismWell> &well) {
  float x = 0, y = 0, z = 0;
  float lx = 0, ly = 0, lz = 0;
  std::tie(lx, ly, lz) = well->getPoint(0);
  for (auto point = ++begin(well->getPoints()); point != end(well->getPoints());
       ++point) {
    std::tie(x, y, z) = *point;
    QVector3D pipeVector =
        vectorBy2Point(QVector3D(lx, ly, lz), QVector3D(x, y, z));
    float scaling = calculateLenght(QVector3D(x, y, z), QVector3D(lx, ly, lz)) /
                    _maxAxisValue;
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

bool OilFieldScene::showEvent(const QUuid &uid) {
  if (_events.end() != _events.find(uid)) {
    _events.at(uid)->setVisible(true);
    return true;
  }
  return false;
}

// bool OilFieldScene::showEvent(const std::unique_ptr<Data::SeismEvent> &event)
// {
//  showEvent(event->getUuid());
//}

void OilFieldScene::setProject(
    const std::unique_ptr<Data::SeismProject> &project) {
  for (auto &uuid_event : project->getAllMap<SeismEvent>()) {
    addEvent(uuid_event.second);
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

// bool OilFieldScene::removeEvent(
//    const std::unique_ptr<Data::SeismEvent> &event) {
//  return removeEvent(event->getUuid());
//}

bool OilFieldScene::removeEvent(const Uuid &uid) {
  if (_events.end() != _events.find(uid)) {
    QCustom3DItem *item = _events[uid];
    if (item == _isItemHanlde) {
      removeItemHandle();
    }
    _surface->removeCustomItem(item);
    _events.erase(uid);
    return true;
  }
  return false;
}

// bool OilFieldScene::removeHorizon(
//    const std::unique_ptr<Data::SeismHorizon> &horizon) {
//  return removeHorizon(horizon->getUuid());
//}

bool OilFieldScene::removeHorizon(const Uuid &uid) {
  if (_horizons.end() != _horizons.find(uid)) {
    _surface->removeSeries(_horizons.at(uid));
    _horizons.erase(uid);
    return true;
  }
  return false;

  //  QSurface3DSeries *series = ;
  //  if (_horizons.erase(uid)) {
  //    _horizons.erase(uid);
  //    return true;
  //  }
  //  return false;
}

// bool OilFieldScene::removeReceiver(
//    const std::unique_ptr<Data::SeismReceiver> &receiver) {
//  return removeReceiver(receiver->getUuid());
//}

bool OilFieldScene::removeReceiver(const Uuid &uid) {
  if (_receivers.end() != _receivers.find(uid)) {
    QCustom3DItem *item = _receivers[uid];
    if (item == _isItemHanlde) {
      removeItemHandle();
    }
    _surface->removeCustomItem(item);
    _receivers.erase(uid);
    return true;
  }
  return false;
}

// bool OilFieldScene::removeWell(const std::unique_ptr<Data::SeismWell> &well)
// {
//  return removeWell(well->getUuid());
//}

bool OilFieldScene::removeWell(const Uuid &uid) {
  if (_wells.end() != _wells.find(uid)) {
    for (auto &item : _wells[uid]) {
      if (item == _isItemHanlde) {
        removeItemHandle();
      }
      _surface->removeCustomItem(item);
    }
    _wells.erase(uid);
    return true;
  }
  return false;
}

bool OilFieldScene::hideEvent(const QUuid &uid) {
  if (_events.end() != _events.find(uid)) {
    auto event = _events.at(uid);
    if (event == _isItemHanlde) {
      removeItemHandle();
    }
    event->setVisible(false);
    return true;
  }
  return false;
}

// bool OilFieldScene::hideEvent(const std::unique_ptr<Data::SeismEvent> &event)
// {
//  if (_events.at(event->getUuid())) {
//    _events.at(event->getUuid())->setVisible(true);
//    return true;
//  }
//  return false;
//}

void OilFieldScene::hideAllEvent(bool hide) {
  _isEventsHide = hide;
  for (auto &uid_event : _events) {
    auto event = uid_event.second;
    if (event == _isItemHanlde) {
      removeItemHandle();
    }
    event->setVisible(!hide);
  }
}

void OilFieldScene::hideAllWell(bool hide) {
  _isWellsHide = hide;
  for (auto &well : _wells) {
    for (auto &wellPart : well.second) {
      if (wellPart == _isItemHanlde) {
        removeItemHandle();
      }
      wellPart->setVisible(!hide);
    }
  }
}

void OilFieldScene::hideAllReceiver(bool hide) {
  _isReceiversHide = hide;
  for (auto &uid_receiver : _receivers) {
    auto receiver = uid_receiver.second;
    if (receiver == _isItemHanlde) {
      removeItemHandle();
      //      _surface->removeCustomItemAt(_label->position());
    }
    receiver->setVisible(!hide);
  }
}

void OilFieldScene::hideAllHorizon(bool hide) {
  _isHorizonsHide = hide;
  for (auto &horizon : _horizons) {
    horizon.second->setVisible(!hide);
  }
}

const std::map<Uuid, QCustom3DItem *> OilFieldScene::getEvents() const {
  return _events;
}

const std::map<Uuid, QSurface3DSeries *> OilFieldScene::getHorizons() const {
  return _horizons;
}

void OilFieldScene::removeItemHandle() {
  assert(nullptr != _isItemHanlde && nullptr != _label);
  _surface->removeCustomItemAt(_label->position());
  _isItemHanlde = nullptr;
  _label = nullptr;
  _isHandle = false;
}

void OilFieldScene::handleElementSelected(QAbstract3DGraph::ElementType type) {
  if (type == QAbstract3DGraph::ElementCustomItem) {
    QCustom3DItem *item = _surface->selectedCustomItem();
    if (_isHandle) {
      removeItemHandle();
    }
    _isItemHanlde = item;
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

void OilFieldScene::settingGraph() {

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

  _surface->axisZ()->setReversed(true);
  // end setting axis

  connect(_surface, &QAbstract3DGraph::selectedElementChanged, this,
          &OilFieldScene::handleElementSelected);
}

void OilFieldScene::checkAxisRange(QCustom3DItem &newItem) {
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

void OilFieldScene::fillSurfaceDataRow(QSurfaceDataArray *dataRow,
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

QSurface3DSeries *
OilFieldScene::createHorizonSeries(QSurfaceDataArray *dataArray) {
  QSurface3DSeries *series = new QSurface3DSeries;
  series->dataProxy()->resetArray(dataArray);
  series->setDrawMode(QSurface3DSeries::DrawSurface);
  series->setBaseColor(_colours[_indexColor++ % 7]);
  return series;
}

QVector3D OilFieldScene::vectorBy2Point(QVector3D from, QVector3D to) {
  float x1 = from.x(), y1 = from.y(), z1 = from.z();
  float x2 = to.x(), y2 = to.y(), z2 = to.z();
  return QVector3D(x2 - x1, y2 - y1, z2 - z1);
}

float OilFieldScene::calculateLenght(QVector3D from, QVector3D to) {
  return static_cast<float>(sqrt((from.x() - to.x()) * (from.x() - to.x()) +
                                 (from.y() - to.y()) * (from.y() - to.y()) +
                                 (from.z() - to.z()) * (from.z() - to.z())));
}

} // namespace Main
