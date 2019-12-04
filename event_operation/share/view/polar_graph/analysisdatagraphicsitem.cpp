#include "analysisdatagraphicsitem.h"

#include <QPainter>

namespace EventOperation {

QRectF AnalysisDataGraphicsItem::boundingRect() const
{
    QPointF anchor = mapFromParent(_chart->mapToPosition(_anchor));
    QRectF rect;
    rect.setLeft(qMin(rect.left(), anchor.x()));
    rect.setRight(qMax(rect.right(), anchor.x()));
    rect.setTop(qMin(rect.top(), anchor.y()));
    rect.setBottom(qMax(rect.bottom(), anchor.y()));
    return rect;
}

void AnalysisDataGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterPath path;
    path.addRoundedRect(_rect, 5, 5);

    QPointF anchor = mapFromParent(_chart->mapToPosition(_anchor));
    if (!_rect.contains(anchor)) {
        QPointF point1, point2;

        // establish the position of the anchor point in relation to m_rect
        bool above = anchor.y() <= _rect.top();
        bool aboveCenter = anchor.y() > _rect.top() && anchor.y() <= _rect.center().y();
        bool belowCenter = anchor.y() > _rect.center().y() && anchor.y() <= _rect.bottom();
        bool below = anchor.y() > _rect.bottom();

        bool onLeft = anchor.x() <= _rect.left();
        bool leftOfCenter = anchor.x() > _rect.left() && anchor.x() <= _rect.center().x();
        bool rightOfCenter = anchor.x() > _rect.center().x() && anchor.x() <= _rect.right();
        bool onRight = anchor.x() > _rect.right();

        // get the nearest m_rect corner.
        qreal x = (onRight + rightOfCenter) * _rect.width();
        qreal y = (below + belowCenter) * _rect.height();
        bool cornerCase = (above && onLeft) || (above && onRight) || (below && onLeft) || (below && onRight);
        bool vertical = qAbs(anchor.x() - x) > qAbs(anchor.y() - y);

        qreal x1 = x + leftOfCenter * 10 - rightOfCenter * 20 + cornerCase * !vertical * (onLeft * 10 - onRight * 20);
        qreal y1 = y + aboveCenter * 10 - belowCenter * 20 + cornerCase * vertical * (above * 10 - below * 20);
        point1.setX(x1);
        point1.setY(y1);

        qreal x2 = x + leftOfCenter * 20 - rightOfCenter * 10 + cornerCase * !vertical * (onLeft * 20 - onRight * 10);
        qreal y2 = y + aboveCenter * 20 - belowCenter * 10 + cornerCase * vertical * (above * 20 - below * 10);
        point2.setX(x2);
        point2.setY(y2);

        path.moveTo(point1);
        path.lineTo(anchor);
        path.lineTo(point2);
        path = path.simplified();
    }
    painter->setBrush(QColor(255, 255, 255));
    painter->drawPath(path);
    painter->drawText(_textRect, _text);
}

AnalysisDataGraphicsItem::AnalysisDataGraphicsItem(QPolarChart *parent) : QGraphicsItem(parent), _chart(parent)
{

}

void AnalysisDataGraphicsItem::setText(const QString &text)
{
    _text = text;
    QFontMetrics metrics(_font);
    _textRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, _text);
    _textRect.translate(5, 5);
    prepareGeometryChange();
    _rect = _textRect.adjusted(-5, -5, 5, 5);
}

void AnalysisDataGraphicsItem::setAnchor(QPointF point)
{
    _anchor = point;
}

void AnalysisDataGraphicsItem::updateGeometry()
{
    prepareGeometryChange();
    setPos(_chart->mapToPosition(_anchor) + QPoint(10, -50));
}

}

