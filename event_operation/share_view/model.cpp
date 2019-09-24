#include "model.h"

#include <QtWidgets/QGesture>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <iostream> // TODO: delete

Model::Model(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags) {
  grabGesture(Qt::PanGesture);
  grabGesture(Qt::PinchGesture);
}

void Model::addWaves(const QRectF &wave) { waves.push_back(wave); }
void Model::addBorders(const QRectF &border) { borders.push_back(border); }

bool Model::isWave(const qreal x, const qreal y) const {
  for (auto rect : waves) {
    if (rect.contains(x, y)) {
      return true;
    }
  }
  return false;
}

bool Model::sceneEvent(QEvent *event) {
  if (event->type() == QEvent::Gesture)
    return gestureEvent(static_cast<QGestureEvent *>(event));
  return QChart::event(event);
}

bool Model::gestureEvent(QGestureEvent *event) {
  if (QGesture *gesture = event->gesture(Qt::PanGesture)) {
    QPanGesture *pan = static_cast<QPanGesture *>(gesture);
    QChart::scroll(-(pan->delta().x()), pan->delta().y());
  }

  if (QGesture *gesture = event->gesture(Qt::PinchGesture)) {
    QPinchGesture *pinch = static_cast<QPinchGesture *>(gesture);
    if (pinch->changeFlags() & QPinchGesture::ScaleFactorChanged)
      QChart::zoom(pinch->scaleFactor());
  }

  return true;
}
