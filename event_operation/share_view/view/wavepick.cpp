#include "wavepick.h"
#include <QtCharts/QChart>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSceneMouseEvent>

namespace EventOperation {
WavePick::WavePick(QChart *chart, QPointF pos, QSize size)
    : QGraphicsItem(chart), _chart(chart), _pos(pos), _size(size) {
  _rect = QRectF(pos.x(), pos.y(), size.width(), size.height());
}

WavePick::WavePick(QChart *chart, qreal ax, qreal ay, int width, int height)
    : QGraphicsItem(chart), _chart(chart), _pos(QPointF(ax, ay)),
      _size(QSize(width, height)) {
  _rect = QRectF(0, 0, 40, 20);
}

void WavePick::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                     QWidget *) {
  QPainterPath path;
  path.addRoundedRect(_rect, 5, 5);
  painter->setBrush(Qt::darkGray);
  painter->drawPath(path);
}

void WavePick::setAnchor(const QPointF point) { _anchor = point; }

void WavePick::updateGeomety() {
  prepareGeometryChange();
  setPos(_chart->mapToPosition(_anchor) + QPoint(-10 * scale(), 50 * scale()));
}

QRectF WavePick::boundingRect() const {
  QPointF anchor = mapFromParent(_chart->mapToPosition(_anchor));
  QRectF rect;
  rect.setLeft(qMin(_rect.left(), _anchor.x()));
  rect.setRight(qMax(_rect.right(), anchor.x()));
  rect.setTop(qMin(_rect.top(), _anchor.y()));
  rect.setBottom(qMax(_rect.bottom(), _anchor.y()));
  return rect;
}

void WavePick::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  event->setAccepted(true);
}

void WavePick::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    setPos(mapToParent(event->pos() - event->buttonDownPos(Qt::LeftButton)));
    event->setAccepted(true);
  } else {
    event->setAccepted(false);
  }
}

void WavePick::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  //  _anchor = mapFromParent(_chart->mapToPosition(_anchor));
}
} // namespace EventOperation
