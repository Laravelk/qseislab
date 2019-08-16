#include "surface.h"

#include "../../data/seismevent.h"
#include "../../data/seismhorizon.h"
#include "../../data/seismproject.h"

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismProject SeismProject;

namespace Main {
Surface::Surface(Q3DSurface *surface) : _surface(surface), _isHandle(false) {
  _blackColor = QImage(2, 2, QImage::Format_RGB32);
  _redColor = QImage(2, 2, QImage::Format_RGB32);
  _blackColor.fill(Qt::black);
  _redColor.fill(Qt::red);

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

void Surface::addHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon) {
  _pointVector = horizon->getPoints();
  for (int i = 0; i < 100; i++) {
    SeismPoint point = _pointVector.at(i);
  }
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
