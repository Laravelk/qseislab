#pragma once
#include <QGraphicsItem>
#include <QtCharts/QChart>

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

namespace EventOperation {
class GraphicsWaveItem : public QGraphicsItem {
public:
  explicit GraphicsWaveItem(QChart *, qreal, qreal, int, int);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;
  QRectF boundingRect() const override;
  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
  QPointF m_shiftMouseCoords;
  qreal _xInCoords;
  qreal _yInCoords;
  int _widthInCoords;
  int _heightInCoords;
  qreal _x;
  qreal _y;
  int _width;
  int _height;

  qreal _xInPlotAreaCoords;
  qreal _yInPlotAreaCoords;
  int _widthInPlotAreaCoords;
  int _heightInPlotAreaCoords;

  QChart *_chart;
};
} // namespace EventOperation
