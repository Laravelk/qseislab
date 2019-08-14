#include "surface.h"

#include "../../data/seismevent.h"
#include "../../data/seismhorizon.h"
#include "../../data/seismproject.h"

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismProject SeismProject;

namespace Main {
Surface::Surface(Q3DSurface *surface) : _surface(surface) {}
Surface::~Surface() { delete _surface; }

void Surface::addEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  //  _event.push_back(std::move(event));
  addEventInGraph(event);
}

void Surface::addHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon) {
  //  _horizon.push_back(std::move(horizon));
  addHorizonInGraph();
}

void Surface::setProject(const std::unique_ptr<Data::SeismProject> &project) {}

void Surface::addEventInGraph(const std::unique_ptr<Data::SeismEvent> &event) {
  QImage color = QImage(2, 2, QImage::Format_RGB32);
  color.fill(Qt::black);
  QVector3D position(0.0f, 0.0f, 0.0f); // TODO read from event
  QCustom3DItem *item = new QCustom3DItem(
      "/Users/ivanmorozov/MyQtProject/Геология/qseislab/resources/"
      "sphereSmooth.obj",
      position, QVector3D(0.025f, 0.025f, 0.025f), QQuaternion(), color);
  item->setShadowCasting(false);
  _surface->addCustomItem(item);
}

void Surface::addHorizonInGraph() {}

} // namespace Main
