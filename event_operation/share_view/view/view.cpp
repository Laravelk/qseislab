#include "view.h"
#include "../model.h"
#include "wavepick.h"

#include <QtGui/QMouseEvent>
#include <iostream> // TODO: delete

namespace EventOperation {
View::View(QChart *chart, QWidget *parent)
    : QChartView(chart, parent), mouseIsTouching(false) {
  chart->setAnimationOptions(QChart::NoAnimation);
  setDragMode(QGraphicsView::NoDrag);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setRenderHint(QPainter::Antialiasing);
  scene()->addItem(chart);
  rect = scene()->addRect(chart->plotArea());
  rect->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
}

void View::addPick(qreal ax, qreal ay, int width, int height) {
  addPick(QPointF(ax, ay), QSize(width, height));
}

void View::addPick(QPointF pos, QSize size) {
  WavePick *pick = new WavePick(chart(), pos, size);
  scene()->addItem(pick);
  wavePicks.push_back(pick);
}

void View::addPick() {
  WavePick *p = new WavePick(chart(), QPointF(20000, 5), QSize(40, 15));
  WavePick *p2 = new WavePick(chart(), QPointF(10000, 5), QSize(40, 15));
  scene()->addItem(p);
  scene()->addItem(p2);
  wavePicks.push_back(p);
  wavePicks.push_back(p2);
}

bool View::viewportEvent(QEvent *event) {

  if (event->type() == QEvent::TouchBegin) {
    mouseIsTouching = true;
  }
  return QChartView::viewportEvent(event);
}

void View::mousePressEvent(QMouseEvent *event) {
  if (mouseIsTouching)
    return;
  QChartView::mousePressEvent(event);
  if (event->button() == Qt::RightButton) {
    for (auto &wave : wavePicks) {
      wave->updateGeomety();
    }
  }
}

void View::mouseMoveEvent(QMouseEvent *event) {
  if (mouseIsTouching)
    return;
  QChartView::mouseMoveEvent(event);
}

void View::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    if (scene()) {
      scaleContentsBy(0.7);
      return;
    }
  }
  if (mouseIsTouching) {
    mouseIsTouching = false;
  }
  QChartView::mouseReleaseEvent(event);
}

void View::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Plus:
    scaleContentsBy(2);
    break;
  case Qt::Key_Minus:
    scaleContentsBy(0.5);
    break;
  case Qt::Key_Left:
    scrollContentsBy(-10, 0);
    break;
  case Qt::Key_A:
    scrollContentsBy(-10, 0);
    break;
  case Qt::Key_Right:
    scrollContentsBy(10, 0);
    break;
  case Qt::Key_D:
    scrollContentsBy(10, 0);
    break;
  case Qt::Key_Up:
    scrollContentsBy(0, 10);
    break;
  case Qt::Key_W:
    scrollContentsBy(0, 10);
    break;
  case Qt::Key_Down:
    scrollContentsBy(0, -10);
    break;
  case Qt::Key_S:
    scrollContentsBy(0, -10);
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
  QChartView::mouseDoubleClickEvent(event);
}

void View::paintEvent(QPaintEvent *event) { QChartView::paintEvent(event); }

void View::scrollContentsBy(int dx, int dy) {
  if (scene()) {
    _chart->scroll(dx, dy);
    for (auto &wave : wavePicks) {
      wave->updateGeomety();
    }
  }
}

void View::resizeEvent(QResizeEvent *event) {
  if (scene()) {
    scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    _chart->resize(event->size());
    for (auto &wave : wavePicks) {
      wave->updateGeomety();
    }
  }
  QGraphicsView::resizeEvent(event);
}

// uncomment for wheelEvent on Windows
void View::wheelEvent(QWheelEvent *event) {
  //  qreal factor = event->angleDelta().y() > 0 ? 0.7 : 1.3;
  //  scaleContentsBy(factor);
  //  QChartView::wheelEvent(event);
}

void View::dragLeaveEvent(QDragLeaveEvent *) { std::cerr << "DRAG"; }

void View::scaleContentsBy(qreal factor) {
  if (scene()) {
    _chart->zoom(factor);
    for (auto &wave : wavePicks) {
      wave->setScale(wave->scale() * factor);
      wave->updateGeomety();
    }
  }
}
} // namespace EventOperation
