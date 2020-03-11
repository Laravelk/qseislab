#include "wavezone.h"

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QChart>

#include <iostream> // TODO: delete

namespace EventOperation {
WaveZone::WaveZone(QChart *chart, QPointF anchor, QGraphicsItem *parent,
                   QSizeF size, QColor color, int n)
    : QGraphicsItem(parent), _chart(chart), _anchor(anchor), _size(size) {
  std::cerr << n << " " << _anchor.x() << " " << _anchor.y() << " "
            << _size.width() << " " << _size.height() << std::endl;

  _rect = QRect(0, 0, _size.width(), _size.height());
  _brush = QBrush(QColor(color.red(), color.green(), color.blue(), 100));
  //  updateGeometry();
}

void WaveZone::setAnchor(const QPointF point) {
  _anchor = point;
  updateGeometry();
}

void WaveZone::setRectSize(const QSizeF size) {
  _size.setWidth(size.width());
  _size.setHeight(size.height());
  _rect.setSize(_size);
  updateGeometry();
}

void WaveZone::setWidth(const float width) {
  _size.setWidth(width);
  _rect.setSize(_size);
  updateGeometry();
}

void WaveZone::setColor(const QColor &color) { _brush.setColor(color); }

void WaveZone::updateGeometry() {
  prepareGeometryChange();
  _rect.moveCenter(QPointF(_size.width() / 2, 0));
  setPos(_chart->mapToPosition(_anchor));
}

QSizeF WaveZone::scallByAxis(QSizeF scaleS) {
  _factorHeight *= scaleS.height();
  _factorWidth *= scaleS.width();
  _size.setWidth(_size.width() * scaleS.width());
  _size.setHeight(_size.height() * scaleS.height());
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
