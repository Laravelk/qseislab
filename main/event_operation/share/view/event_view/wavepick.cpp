#include "wavepick.h"

#include <QtCharts/QChart>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <type_traits>

#include <iostream> // TODO: remove

namespace EventOperation {
WavePick::WavePick(Data::SeismWavePick::Type type, QGraphicsItem *parent,
                   QChart *chart, QPointF pos, QSizeF size, QBrush brush,
                   QPointF leftBorder, QPointF rightBorder)
    : QGraphicsItem(parent), _chart(chart), _pos(pos), _size(size),
      _pickBrush(brush), DEFAULT_SIZE(size), MAX_WIDTH(size.width()),
      _leftBorderAnchor(leftBorder), _rightBorderAnchor(rightBorder) {
  setFlag(QGraphicsItem::ItemIgnoresTransformations);
  _type = type;
  _anchor = pos;
  setPos(_anchor);

  _fillZoneBrush = QColor(brush.color().red(), brush.color().green(),
                          brush.color().blue(), 100);
  if (type == Data::SeismWavePick::Type::PWAVE) {
    _borderBrush = Qt::darkGreen;
  } else {
    _borderBrush = Qt::darkMagenta;
  }

  _rect = QRectF(0, 0, size.width(), size.height());
  _rightBorderRect = QRectF(0, 0, size.width(), size.height());
  _leftBorderRect = QRectF(0, 0, size.width(), size.height());
  _leftFillRect = QRectF(0, 0, size.width(), size.height());
  _rightFillRect = QRectF(0, 0, size.width(), size.height());
  updateGeometry();
}

void WavePick::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                     QWidget *) {
  QPainterPath path;
  QPainterPath rightBorderPath;
  QPainterPath leftBorderPath;
  QPainterPath fillPath;
  QPainterPath p;
  path.addRect(_rect);
  rightBorderPath.addRect(_rightBorderRect);
  leftBorderPath.addRect(_leftBorderRect);
  fillPath.addRect(_leftFillRect);
  p.addRect(_rightFillRect);

  painter->setPen(Qt::NoPen);
  painter->setBrush(_pickBrush);
  painter->drawPath(path);
  painter->setBrush(_borderBrush);
  painter->drawPath(rightBorderPath);
  painter->drawPath(leftBorderPath);
  painter->setBrush(_fillZoneBrush);
  painter->drawPath(fillPath);
  painter->drawPath(p);
}

void WavePick::resize(QSizeF scaleC) {
  _size =
      QSizeF(_size.width() * scaleC.width(), _size.height() * scaleC.height());
  _rect.setSize(_size);
  _leftBorderRect.setSize(_size);
  _rightBorderRect.setSize(_size);
}

void WavePick::setAnchor(const QPointF point) { _anchor = point; }

void WavePick::setLeftBorderAnchor(const QPointF point) {
  _leftBorderAnchor = point;
}

void WavePick::setRightBorderAnchor(const QPointF point) {
  _rightBorderAnchor = point;
}

void WavePick::updateGeometry() {
  prepareGeometryChange();
  _rect.moveCenter(QPointF(0, 0));
  updateWaveZone();

  _leftFillRect.moveCenter(
      (-1) * QPointF((_chart->mapToPosition(_anchor).x() -
                      _chart->mapToPosition(_leftBorderAnchor).x()) /
                         2,
                     0));

  _rightFillRect.moveCenter(
      QPointF((_chart->mapToPosition(_rightBorderAnchor).x() -
               _chart->mapToPosition(_anchor).x()) /
                  2,
              0));

  _leftBorderRect.moveCenter(
      QPointF((-1) * (_chart->mapToPosition(_anchor).x() -
                      _chart->mapToPosition(_leftBorderAnchor).x()),
              0));
  _rightBorderRect.moveCenter(
      QPointF((-1) * _chart->mapToPosition(_anchor).x() +
                  _chart->mapToPosition(_rightBorderAnchor).x(),
              0));

  setPos(_chart->mapToPosition(_anchor));
}

void WavePick::updateWaveZone() {
  _leftFillRect.setWidth(
      _chart->mapToPosition(QPointF(_anchor.x(), 0)).x() -
      _chart->mapToPosition(QPointF(_leftBorderAnchor.x(), 0)).x());
  _rightFillRect.setWidth(
      _chart->mapToPosition(QPointF(_rightBorderAnchor.x(), 0)).x() -
      _chart->mapToPosition(QPointF(_anchor.x(), 0)).x());
}

QSizeF WavePick::scallByAxis(QSizeF scaleS) {
  qreal scaleY = scaleS.height();
  _size.setWidth(3); // some optimal const
  _size.setHeight(_size.height() * scaleY);

  _rect.setSize(_size);
  _leftBorderRect.setSize(_size);
  _rightBorderRect.setSize(_size);
  _leftFillRect.setSize(
      QSizeF(_leftFillRect.size().width() * scaleS.width(),
             _leftFillRect.size().height() * scaleS.height()));
  _rightFillRect.setSize(
      QSizeF(_rightFillRect.size().width() * scaleS.width(),
             _rightFillRect.size().height() * scaleS.height()));
  return _rect.size();
}

// rect from left border to right border.
QRectF WavePick::boundingRect() const {
  QPointF anchor = mapFromParent(_chart->mapToPosition(_anchor));
  QPointF leftBorderAnchor =
      mapFromParent(_chart->mapToPosition(_leftBorderAnchor));
  QPointF rightBorderAnchor =
      mapFromParent(_chart->mapToPosition(_rightBorderAnchor));
  QRectF rect;
  rect.setLeft(qMin(qMin(_rect.left(), anchor.x()),
                    qMin(_rect.left(), leftBorderAnchor.x())));
  rect.setRight(qMax(qMax(_rect.right(), anchor.x()),
                     qMax(_rect.right(), rightBorderAnchor.x())));
  rect.setTop(qMin(_rect.top(), _anchor.y()));
  rect.setBottom(qMax(_rect.bottom(), _anchor.y()));
  return rect;
}

void WavePick::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->buttons() == Qt::RightButton &&
      (Qt::AltModifier == QGuiApplication::keyboardModifiers()) &&
      abs(_anchor.x() - _chart->mapToValue(mapToParent(event->pos())).x()) <=
          DRAG_OFFSET) {
    emit deleted();
  }
  event->setAccepted(true);
}

void WavePick::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton &&
      (Qt::AltModifier == QGuiApplication::keyboardModifiers())) {
    QPointF newPosition =
        QPointF(_chart->mapToValue(mapToParent(event->pos())));
    if (abs(_anchor.x() - _chart->mapToValue(mapToParent(event->pos())).x()) <=
            DRAG_OFFSET ||
        NOW_MOVE::WAVE == _move) {
      _move = NOW_MOVE::WAVE;
      if (newPosition.x() < _rightBorderAnchor.x() &&
          newPosition.x() > _leftBorderAnchor.x()) {
        setPos(QPointF(
            mapToParent(event->pos() - event->buttonDownPos(Qt::LeftButton))
                .x(),
            pos().y()));
        _anchor = QPointF(newPosition.x(), _anchor.y());
        updateGeometry();
      }
      return;
    }

    if (abs(_leftBorderAnchor.x() -
            _chart->mapToValue(mapToParent(event->pos())).x()) <= DRAG_OFFSET ||
        NOW_MOVE::L_BORDER == _move) {
      _move = L_BORDER;
      if (newPosition.x() < _anchor.x() && newPosition.x() > 0) {
        setPos(QPointF(mapToParent(event->pos()).x(), pos().y()));
        _leftBorderAnchor = QPointF(newPosition.x(), _anchor.y());
        updateGeometry();
      }
      return;
    }
    if (abs(_rightBorderAnchor.x() -
            _chart->mapToValue(mapToParent(event->pos())).x()) <= DRAG_OFFSET ||
        NOW_MOVE::R_BORDER == _move) {
      _move = R_BORDER;
      if (newPosition.x() > _anchor.x()) {
        setPos(QPointF(mapToParent(event->pos()).x(), pos().y()));
        _rightBorderAnchor = QPointF(newPosition.x(), _anchor.y());
        updateGeometry();
      }
    }
    return;
  }
}

void WavePick::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  if (NOW_MOVE::NONE != _move) {
    _move = NOW_MOVE::NONE;
    emit changed(_leftBorderAnchor.x(), _anchor.x(), _rightBorderAnchor.x());
  }
}

void WavePick::mouseDoubleClickEvent(
    QGraphicsSceneMouseEvent *event) // second variant delete pick
{
  qreal newX = QPointF(_chart->mapToValue(mapToParent(event->pos()) -
                                          event->buttonDownPos(Qt::LeftButton)))
                   .x();
  _anchor = QPointF(newX, _anchor.y());
}

} // namespace EventOperation
