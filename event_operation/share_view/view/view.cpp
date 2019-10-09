#include "view.h"
#include "wavepick.h"

#include <QtGui/QMouseEvent>
#include <iostream> // TODO: delete
#include <math.h>

namespace EventOperation {
View::View(QChart *chart, QWidget *parent)
    : QChartView(chart, parent), mouseIsTouching(false) {
  chart->setAnimationOptions(QChart::NoAnimation);
  setDragMode(QGraphicsView::NoDrag);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setRenderHint(QPainter::Antialiasing);
  chart->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
  scene()->addItem(chart);
}

void View::addPick(qreal ax, qreal ay, int width, int height, QBrush brush,
                   qreal rangeX) {
  addPick(QPointF(ax, ay), QSize(width, height), brush, rangeX);
}

void View::addPick(QPointF pos, QSize size, QBrush brush, qreal rangeX) {
  QBrush borderBrush;
  if (brush == Qt::darkRed) {
    borderBrush = Qt::darkGreen;
  } else {
    borderBrush = Qt::darkCyan;
  }
  WavePick *pick = new WavePick(chart(), pos, size, brush, 2, 4);
  connect(pick, &WavePick::sendTypeNumCompY,
          [this](auto type, auto num, auto newPos) {
            emit sendTypeNumCompY(type, num, newPos);
          });
  WavePick *leftBorder = new WavePick(
      chart(), QPointF(pos.x() - 40000, pos.y()), size, borderBrush, 0, pick);
  WavePick *rightBorder =
      new WavePick(chart(), QPointF(pos.x() + 40000, pos.y()), size,
                   borderBrush, pick, rangeX);
  pick->setBorders(leftBorder, rightBorder);
  pick->setZValue(11);
  leftBorder->setZValue(11);
  rightBorder->setZValue(11);
  _wavePicks.push_back(leftBorder);
  _wavePicks.push_back(rightBorder);
  _wavePicks.push_back(pick);
}

void View::setAddPickFlag(bool t) { addPickButtonPress = t; }

bool View::viewportEvent(QEvent *event) {

  if (event->type() == QEvent::TouchBegin) {
    mouseIsTouching = true;
  }
  return QChartView::viewportEvent(event);
}

void View::mousePressEvent(QMouseEvent *event) {
  if (addPickButtonPress) {
    QPointF pos = calculatePickPosition(chart()->mapToValue(event->pos()));
    addPick(pos, QSize(5, 40), Qt::darkRed, _rangeX);
    addPickButtonPress = false;
  }
  QChartView::mousePressEvent(event);
  if (event->button() == Qt::RightButton) {
    for (auto &wave : _wavePicks) {
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
    for (auto &wave : _wavePicks) {
      wave->updateGeomety();
    }
  }
}

void View::resizeEvent(QResizeEvent *event) {
  if (scene()) {
    scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    _chart->resize(event->size());
    for (auto &wave : _wavePicks) {
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

void View::scaleContentsBy(qreal factor) {
  if (scene()) {
    _chart->zoom(factor);
    for (auto &wave : _wavePicks) {
      wave->setScale(wave->scale() * factor);
      wave->updateGeomety();
    }
  }
}

QPointF View::calculatePickPosition(QPointF pointByMouse) {
  if (pointByMouse.y() > 7) {
    return QPointF(pointByMouse.x() - 500, 7 + WAVE_RADIUS);
  }

  if (pointByMouse.y() < 0) {
    return QPointF(pointByMouse.x() - 500, 0 + WAVE_RADIUS);
  }

  return QPointF(pointByMouse.x() - 500, round(pointByMouse.y()) + WAVE_RADIUS);
}
} // namespace EventOperation
