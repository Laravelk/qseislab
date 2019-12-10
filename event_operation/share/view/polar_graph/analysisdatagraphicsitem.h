#pragma once

#include <QBrush>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QPolarChart>
#include <QtWidgets/QGraphicsItem>

QT_BEGIN_NAMESPACE
class QGraphicsSceneEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE;

namespace EventOperation {
class AnalysisDataGraphicsItem : public QGraphicsItem
{
public:
    AnalysisDataGraphicsItem(QPolarChart *parent);
    void setText(const QString &text);
    void setAnchor(QPointF point);
    void setRectAnchor(QPointF point);
    void updateGeometry();

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QString _text;
    QRectF _textRect;
    QPointF _anchor;
    QPointF _rectAnchor;
    QRectF _rect;
    QFont _font;
    QPolarChart *_chart;
};
}

