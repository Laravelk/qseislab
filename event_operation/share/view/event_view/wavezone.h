#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include <QChart>

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE;

namespace EventOperation {
class WaveZone : public QObject, public QGraphicsItem {
  Q_OBJECT
public:
  WaveZone(QChart *, QPointF, QGraphicsItem *, QSizeF, QColor, int);
  void setAnchor(const QPointF);

  void setRectSize(const QSizeF);
  void setWidth(const float);
  void setColor(const QColor &color);
  void setNum(const int a) { num = a; }

  void updateGeometry();
  QSizeF scallByAxis(QSizeF);
  QRectF boundingRect() const;

  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

protected:
private:
  float _factorWidth = 1.0f;
  float _factorHeight = 1.0f;
  QPointF _anchor;
  QRectF _rect;
  QBrush _brush;
  QChart *_chart;
  QSizeF _size;
  int num = 0; // TODO: delete
};
}
