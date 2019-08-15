#include "surface.h"

#include "../../data/seismevent.h"
#include "../../data/seismhorizon.h"
#include "../../data/seismproject.h"

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismProject SeismProject;

namespace Main {
Surface::Surface(Q3DSurface *surface) : _surface(surface) {
  _blackColor = QImage(2, 2, QImage::Format_RGB32);
  _redColor = QImage(2, 2, QImage::Format_RGB32);
  _blackColor.fill(Qt::black);
  _redColor.fill(Qt::red);
}

Surface::~Surface() {
  _surface->removeCustomItems();
  delete _surface;
}

void Surface::addEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  addEventInGraph(event);
}

void Surface::addHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon) {
  addHorizonInGraph(horizon);
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

const std::map<Uuid, QSurface3DSeries *> Surface::getHorizonMap() const {
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

void Surface::addHorizonInGraph(const std::unique_ptr<SeismHorizon> &horizon) {}

} // namespace Main
