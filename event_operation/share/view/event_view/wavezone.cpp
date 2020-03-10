#include "wavezone.h"

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QChart>

namespace EventOperation {
WaveZone::WaveZone(QChart *chart, QPointF anchor, QGraphicsItem *parent,
                   QSizeF size)
    : QGraphicsItem(parent), _chart(chart), _anchor(anchor), _size(size) {
  _rect = QRect(0, 0, _size.width(), _size.height());
  _brush = QBrush(QColor(30, 30, 30, 100));
  updateGeometry();
}

void WaveZone::setAnchor(const QPointF point) { _anchor = point; }

void WaveZone::updateGeometry() {
  prepareGeometryChange();
  _rect.moveCenter(QPointF(_size.width() / 2, 0));
  setPos(_chart->mapToPosition(_anchor));
}

QSizeF WaveZone::scallByAxis(QSizeF scaleS) {
  qreal scaleY = scaleS.height();
  _size.setWidth(_size.width() * scaleS.width());
  _size.setHeight(_size.height() * scaleY);
  _rect.setSize(_size);
  return _rect.size();
}

QRectF WaveZone::boundingRect() const {
  QPointF anchor = mapFromParent(_chart->mapToPosition(_anchor));
  QRectF rect;
  rect.setLeft(qMin(_rect.left(), _anchor.x()));
  rect.setRight(qMax(_rect.right(), anchor.x()));
  rect.setTop(qMin(_rect.top(), _anchor.y()));
  rect.setBottom(qMax(_rect.bottom(), _anchor.y()));
  return rect;
}

void WaveZone::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                     QWidget *) {
  QPainterPath path;
  path.addRect(_rect);
  painter->setPen(Qt::NoPen);
  painter->setBrush(_brush);
  painter->drawPath(path);
}
}
