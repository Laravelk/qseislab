#include "chartgesture.h"

#include <QtWidgets/QGesture>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>

#include <iostream> // TODO: delete

#include "graphic_view/wavepick.h"

namespace EventOperation {
ChartGesture::ChartGesture(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags) {
  grabGesture(Qt::PanGesture);
  grabGesture(Qt::PinchGesture);
}

bool ChartGesture::sceneEvent(QEvent *event) {
  if (event->type() == QEvent::Gesture) {
    return gestureEvent(static_cast<QGestureEvent *>(event));
  }
  return QChart::event(event);
}

void ChartGesture::zoom(qreal factor) { QChart::zoom(factor); }

void ChartGesture::zoomIn(const QRectF &rect) { QChart::zoomIn(rect); }

bool ChartGesture::gestureEvent(QGestureEvent *event) {
  /*if (QGesture *gesture = event->gesture(Qt::PanGesture)) {
    QPanGesture *pan = static_cast<QPanGesture *>(gesture);
    QChart::scroll(-(pan->delta().x()), pan->delta().y());
    for (auto &it : *_wavePicks) {
      it->updateGeometry();
    }
  }

  if (QGesture *gesture = event->gesture(Qt::PinchGesture)) {
    QPinchGesture *pinch = static_cast<QPinchGesture *>(gesture);
    if (pinch->changeFlags() & QPinchGesture::ScaleFactorChanged) {
      QChart::zoom(pinch->scaleFactor());
      for (auto &it : *_wavePicks) {
        it->setScale(it->scale() * pinch->scaleFactor());
        it->updateGeometry();
      }
    }
  }
  return tr*/
  ue;
}
} // namespace EventOperation
