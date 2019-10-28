#include "pipes2dname.h"
#include <QtCharts/QChart>
#include <QtGui/QFontMetrics>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <iostream>

namespace EventOperation {
Pipes2DName::Pipes2DName(QChart *chart) : QGraphicsItem(chart), _chart(chart) {}

QRectF Pipes2DName::boundingRect() const {}

void Pipes2DName::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  painter->drawText(_textRect, _text);
}

void Pipes2DName::setText(const QString &text) {
  _text = text;
  QFontMetrics metrics(_font);
  _textRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, _text);
  _textRect.translate(5, 5);
  prepareGeometryChange();
}

void Pipes2DName::setAnchor(QPointF point) { _anchor = point; }

void Pipes2DName::updateGeometry() {
  prepareGeometryChange();
  setPos(_chart->mapToScene(_anchor));
}
} // namespace EventOperation
