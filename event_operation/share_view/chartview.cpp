#include "chartview.h"

#include <QtGui/QMouseEvent>

namespace EventOperation {

ChartView::ChartView(QChart *chart, QWidget *parent)
    : QChartView(chart, parent), m_isTouching(false) {
  setRubberBand(QChartView::RectangleRubberBand);
}

bool ChartView::viewportEvent(QEvent *event) {
  if (event->type() == QEvent::TouchBegin) {
    m_isTouching = true;
    chart()->setAnimationOptions(QChart::NoAnimation);
  }
  return QChartView::viewportEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent *event) {
  if (m_isTouching)
    return;
  QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event) {
  if (m_isTouching)
    return;
  QChartView::mouseMoveEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event) {
  if (m_isTouching)
    m_isTouching = false;
  chart()->setAnimationOptions(QChart::NoAnimation);

  QChartView::mouseReleaseEvent(event);
}

void ChartView::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Plus:
    chart()->zoomIn();
    break;
  case Qt::Key_Minus:
    chart()->zoomOut();
    break;
  case Qt::Key_Left:
    chart()->scroll(-10, 0);
    break;
  case Qt::Key_A:
    chart()->scroll(-10, 0);
    break;
  case Qt::Key_Right:
    chart()->scroll(10, 0);
    break;
  case Qt::Key_D:
    chart()->scroll(10, 0);
    break;
  case Qt::Key_Up:
    chart()->scroll(0, 10);
    break;
  case Qt::Key_W:
    chart()->scroll(0, 10);
    break;
  case Qt::Key_Down:
    chart()->scroll(0, -10);
    break;
  case Qt::Key_S:
    chart()->scroll(0, -10);
    break;
  default:
    QGraphicsView::keyPressEvent(event);
    break;
  }
}

void ChartView::wheelEvent(QWheelEvent *event) {
  //    chart()->zoomReset();

  //    _mFactor *= event->angleDelta().y() > 0 ? 0.5 : 2;

  //    QRectF rect = chart()->plotArea();
  //    QPointF c = chart()->plotArea().center();
  //    rect.setWidth(_mFactor*rect.width());
  //    rect.moveCenter(c);
  //    chart()->zoomIn(rect);

  //    QChartView::wheelEvent(event);
}

} // namespace EventOperation
