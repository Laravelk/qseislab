#include "view.h"

#include <QtGui/QMouseEvent>
#include <iostream> // TODO: delete

View::View(QChart *chart, QWidget *parent)
    : QChartView(chart, parent), mouseIsTouching(false) {
  setRubberBand(QChartView::RectangleRubberBand);
}

void View::addWaveRect(QRect rect) { _qRectVector.push_back(rect); }

bool View::viewportEvent(QEvent *event) {
  if (event->type() == QEvent::TouchBegin) {
    mouseIsTouching = true;
    chart()->setAnimationOptions(QChart::NoAnimation);
  }
  return QChartView::viewportEvent(event);
}

void View::mousePressEvent(QMouseEvent *event) {
  if (mouseIsTouching)
    return;
  QChartView::mousePressEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event) {
  if (mouseIsTouching)
    return;
  QChartView::mouseMoveEvent(event);
}

void View::mouseReleaseEvent(QMouseEvent *event) {
  if (mouseIsTouching)
    mouseIsTouching = false;
  chart()->setAnimationOptions(QChart::NoAnimation);

  QChartView::mouseReleaseEvent(event);
}

void View::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Alt:
    altIsTouching = true;
    break;
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

void View::keyReleaseEvent(QKeyEvent *event) {
  if (altIsTouching) {
    altIsTouching = false;
    std::cerr << "F";
  }
  QChartView::keyReleaseEvent(event);
}

void View::wheelEvent(QWheelEvent *event) {
  //  chart()->zoomReset();

  //  _mFactor *= event->angleDelta().y() > 0 ? 0.5 : 2;

  //  QRectF rect = chart()->plotArea();
  //  QPointF c = chart()->plotArea().center();
  //  rect.setWidth(_mFactor * rect.width());
  //  rect.moveCenter(c);
  //  chart()->zoomIn(rect);

  QChartView::wheelEvent(event);
}