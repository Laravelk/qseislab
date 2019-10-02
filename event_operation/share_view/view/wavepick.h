#pragma once

#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsItem>

QT_BEGIN_NAMESPACE
class QGraphicsSceneEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE;

namespace EventOperation {
class WavePick : public QGraphicsItem {
public:
  WavePick(QChart *, QPointF, QSize);
  WavePick(QChart *, qreal, qreal, int, int);

  void setAnchor(const QPointF);
  void updateGeomety();

  QRectF boundingRect() const;
  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

private:
  QChart *_chart;
  QPointF _pos;
  QSize _size;
  QPointF _anchor;
  QRectF _rect;
};
} // namespace EventOperation
