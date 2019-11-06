#include "wavepick.h"

#include <QtCharts/QChart>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <type_traits>

#include <iostream> // TODO: remove

namespace EventOperation {
WavePick::WavePick(Data::SeismWavePick::Type type, QGraphicsItem *parent,
                   QChart *chart, QPointF pos, QSizeF size, QBrush brush,
                   std::variant<WavePick *, qreal> leftBorder,
                   std::variant<WavePick *, qreal> rightBorder)
    : QGraphicsItem(parent), _chart(chart), _pos(pos), _size(size),
      _leftBorder(leftBorder), _rightBorder(rightBorder), _brush(brush) {
  setFlag(QGraphicsItem::ItemIgnoresTransformations);
  _type = type;
  _anchor = pos;
  setPos(_anchor);
  updateBorders();
  _rect = QRectF(0, 0, size.width(), size.height());
  updateGeometry();
}

WavePick::WavePick(Data::SeismWavePick::Type type, QGraphicsItem *parent,
                   QChart *chart, qreal ax, qreal ay, int width, int height,
                   QBrush brush, WavePick *pick)
    : QGraphicsItem(parent), _chart(chart), _pos(QPointF(ax, ay)),
      _size(QSize(width, height)), _brush(brush) {
  _type = type;
  _anchor = QPointF(ax, ay);
  setPos(_anchor);
  _rect = QRectF(0, 0, width, height);
  updateGeometry();
}

void WavePick::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                     QWidget *) {
  QPainterPath path;
  path.addRect(_rect);
  painter->setPen(Qt::NoPen);
  painter->setBrush(_brush);
  painter->drawPath(path);
}

void WavePick::resize(QSizeF scaleC) {
  _size =
      QSizeF(_size.width() * scaleC.width(), _size.height() * scaleC.height());
  _rect.setSize(_size);
}

void WavePick::setAnchor(const QPointF point) { _anchor = point; }

void WavePick::updateGeometry() {
  prepareGeometryChange();
  _rect.moveCenter(QPointF(0, 0));
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
//        std::cerr << _valueLeftBorder << " " << newPosition.x() << " " <<
//        _valueRightBorder << std::endl;
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

  emit changed();
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
