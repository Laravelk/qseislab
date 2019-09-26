#include "view.h"
#include "../model.h"

#include <QtGui/QMouseEvent>
#include <iostream> // TODO: delete

View::View(QChart *chart, QWidget *parent)
    : QChartView(chart, parent), mouseIsTouching(false) {
  setRubberBand(QChartView::RectangleRubberBand);
  setDragMode(ScrollHandDrag); // sceneRectChange
  connect(this->scene(), SIGNAL(sceneRectChanged(QRectF &)), this,
          SLOT(f(QRectF)));
}

bool View::viewportEvent(QEvent *event) {
  if (event->type() == QEvent::TouchBegin) {
    mouseIsTouching = true;
    chart()->setAnimationOptions(QChart::NoAnimation);
  }
  return QChartView::viewportEvent(event);
}

void View::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    for (auto &it : _model->items) {
      it->setPos(0, 0);
    }
  }
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
  QChartView::keyReleaseEvent(event);
}

void View::mouseDoubleClickEvent(QMouseEvent *event) {
  _model->setMinimumHeight(1000);
  QChartView::mouseDoubleClickEvent(event);
}

void View::f(QRectF &) { std::cerr << "2222222222"; }

void View::wheelEvent(QWheelEvent *event) { QChartView::wheelEvent(event); }
