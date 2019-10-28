#pragma once

#include <QFont>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsTextItem>

QT_BEGIN_NAMESPACE
class QGraphicsSceneEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE;

namespace EventOperation {
class Pipes2DName : public QGraphicsItem {
public:
  Pipes2DName(QChart *);
  void setAnchor(const QPointF anchor);
  void updateGeometry();
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
  void resize(const QSizeF);
  void setText(const QString &);
  QRectF boundingRect() const override;

private:
  QPointF _anchor = {10,310};
  QString _text = "HELLO";
  QRectF _textRect;
  QFont _font;
  QSizeF _size;
  QChart *_chart;
};
} // namespace EventOperation
