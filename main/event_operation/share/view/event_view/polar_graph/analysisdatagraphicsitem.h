#pragma once

#include <QtCharts/QChartGlobal>
#include <QtGui/QFont>
#include <QtWidgets/QGraphicsItem>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class AnalysisDataGraphicItem : public QGraphicsItem {
public:
  AnalysisDataGraphicItem(QChart *parent);

  void setText(const QString &text);
  void setAnchor(QPointF point);
  void updateGeometry();

  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  bool _isMoving;
  QPointF _lastClickPosition;
  QString m_text;
  QRectF m_textRect;
  QRectF m_rect;
  QPointF m_anchor;
  QFont m_font;
  QChart *m_chart;
};
