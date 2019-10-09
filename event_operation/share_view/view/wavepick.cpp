#include "wavepick.h"

#include <QtCharts/QChart>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <type_traits>
#include <variant>

namespace EventOperation {
WavePick::WavePick(QChart *chart, QPointF pos, QSize size, QBrush brush,
                   std::variant<WavePick *, qreal> leftBorder,
                   std::variant<WavePick *, qreal> rightBorder)
    : QGraphicsItem(chart), _chart(chart), _pos(pos), _size(size),
      _brush(brush), _leftBorder(leftBorder), _rightBorder(rightBorder) {
  _anchor = pos;
  setPos(_anchor);
  updateBorders();
  _rect = QRectF(0, 0, size.width(), size.height());
}

WavePick::WavePick(QChart *chart, qreal ax, qreal ay, int width, int height,
                   QBrush brush, WavePick *pick)
    : QGraphicsItem(chart), _chart(chart), _pos(QPointF(ax, ay)),
      _size(QSize(width, height)), _brush(brush) {
  _anchor = QPointF(ax, ay);
  setPos(_anchor);
  _rect = QRectF(0, 0, width, height);
}

void WavePick::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                     QWidget *) {
  QPainterPath path;
  path.addRoundedRect(_rect, 5, 5);
  painter->setBrush(_brush);
  painter->drawPath(path);
}

void WavePick::setAnchor(const QPointF point) { _anchor = point; }

void WavePick::updateGeomety() {
  prepareGeometryChange();
  setPos(_chart->mapToPosition(_anchor));
}

void WavePick::setLeftBorder(std::variant<WavePick *, qreal> border) {
  _leftBorder = border;
}

void WavePick::setRightBorder(std::variant<WavePick *, qreal> border) {
  _rightBorder = border;
}

void WavePick::setBorders(std::variant<WavePick *, qreal> left,
                          std::variant<WavePick *, qreal> right) {
  _leftBorder = left;
  _rightBorder = right;
}

QRectF WavePick::boundingRect() const {
  QPointF anchor = mapFromParent(_chart->mapToPosition(_anchor));
  QRectF rect;
  rect.setLeft(qMin(_rect.left(), _anchor.x()));
  rect.setRight(qMax(_rect.right(), anchor.x()));
  rect.setTop(qMin(_rect.top(), _anchor.y()));
  rect.setBottom(qMax(_rect.bottom(), _anchor.y()));
  return rect;
}

void WavePick::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  updateBorders();
  event->setAccepted(true);
}

void WavePick::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    QPointF newPosition =
        QPointF(_chart
                    ->mapToValue(mapToParent(event->pos()) -
                                 event->buttonDownPos(Qt::LeftButton))
                    .x(),
                _anchor.y());
    if (newPosition.x() < _valueRightBorder &&
        newPosition.x() > _valueLeftBorder) {
      setPos(QPointF(
          mapToParent(event->pos() - event->buttonDownPos(Qt::LeftButton)).x(),
          pos().y()));
    }
    event->setAccepted(true);
  } else {
    event->setAccepted(false);
  }
}

void WavePick::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  qreal newX = QPointF(_chart->mapToValue(mapToParent(event->pos()) -
                                          event->buttonDownPos(Qt::LeftButton)))
                   .x();
  _anchor = QPointF(newX, _anchor.y());

  // TODO: send valid new value
  emit sendTypeNumCompY(Data::SeismWavePick::Type::PWAVE, _anchor.y(), 67);
}

void WavePick::updateBorders() {
  auto border_visitor = [](auto &&arg) -> qreal {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, qreal>) {
      return arg;
    } else if constexpr (std::is_same_v<T, WavePick *>) {
      return arg->getXPos();
    }
  };
  _valueLeftBorder = std::visit(border_visitor, _leftBorder);
  _valueRightBorder = std::visit(border_visitor, _rightBorder);
}

} // namespace EventOperation
