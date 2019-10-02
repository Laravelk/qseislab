#include "graphicswaveitem.h"
#include <QtWidgets>
#include <iostream> // TODO: delete

// WavePick

namespace EventOperation {
GraphicsWaveItem::GraphicsWaveItem(QChart *chart, qreal x, qreal y, int width,
                                   int height, qreal)
    : QGraphicsItem(chart), _x(x), _y(y), _width(width), _height(height),
      _chart(chart) {
  setFlags(ItemIsSelectable | ItemIsMovable); // ItemIgnoresTransformations
  setAcceptDrops(true);
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::LeftButton);
}

void GraphicsWaveItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *, QWidget *) {
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(Qt::darkGray);
  painter->drawRect(QRectF(_x, _y, _width, _height));
}

QRectF GraphicsWaveItem::boundingRect() const {
  return QRectF(_x, _y, _width, _height);
}

void GraphicsWaveItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  m_shiftMouseCoords = pos() - mapToScene(event->pos());
  this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void GraphicsWaveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
  setCursor(QCursor(Qt::ArrowCursor));
}

void GraphicsWaveItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {}

void GraphicsWaveItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseDoubleClickEvent(event);
}
} // namespace EventOperation
