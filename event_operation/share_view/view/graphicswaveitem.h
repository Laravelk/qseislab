#pragma once

#include <QGraphicsItem>

namespace EventOperation {
class GraphicsWaveItem : public QGraphicsItem {
public:
  explicit GraphicsWaveItem(qreal, qreal, int, int);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;
  QRectF boundingRect() const override;
  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
  QPointF m_shiftMouseCoords;
  qreal _x;
  qreal _y;
  int _width;
  int _height;
};
} // namespace EventOperation
