#include <QtWidgets>

#include "graphicswaveitem.h"

namespace EventOperation {
GraphicsWaveItem::GraphicsWaveItem(qreal x, qreal y, int width, int height)
    : _x(x), _y(y), _width(width), _height(height) {
  setFlags(ItemIsSelectable | ItemIsMovable | ItemIgnoresTransformations);
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::LeftButton);
}

void GraphicsWaveItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *, QWidget *) {
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(Qt::darkGray);
  painter->drawRect(QRectF(_x, _y, _width, _height));
}

QRectF GraphicsWaveItem::boundingRect() const { return QRectF(0, 0, 250, 250); }

void GraphicsWaveItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);
  update();
}

void GraphicsWaveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseReleaseEvent(event);
}

void GraphicsWaveItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  qreal newX = event->scenePos().x();
  setPos(newX, _y);
}

void GraphicsWaveItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseDoubleClickEvent(event);
}
} // namespace EventOperation
