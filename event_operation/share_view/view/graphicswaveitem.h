#pragma once

#include <QGraphicsItem>

namespace EventOperation {
class GraphicsWaveItem : public QGraphicsItem {
public:
  explicit GraphicsWaveItem(qreal, qreal, int, int);
  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;
  QRectF boundingRect() const override;

  //  static int sceneWidth;
  //  static int sceneHeight;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;

private:
  qreal _x;
  qreal _y;
  int _width;
  int _height;
};
} // namespace EventOperation
