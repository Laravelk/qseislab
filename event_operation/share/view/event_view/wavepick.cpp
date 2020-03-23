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
      _brush(brush), DEFAULT_SIZE(size), MAX_WIDTH(size.width()),
      _leftBorderAnchor(leftBorder), _rightBorderAnchor(rightBorder) {
  setFlag(QGraphicsItem::ItemIgnoresTransformations);
  _type = type;
  _anchor = pos;
  setPos(_anchor);
  _rect = QRectF(0, 0, size.width(), size.height());
  _rightBorderRect = QRectF(0, 0, size.width(), size.height());
  _leftBorderRect = QRectF(0, 0, size.width(), size.height());
  updateGeometry();
}

void WavePick::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                     QWidget *) {
  QPainterPath path;
  QPainterPath rightBorderPath;
  QPainterPath leftBorderPath;
  path.addRect(_rect);
  rightBorderPath.addRect(_rightBorderRect);
  leftBorderPath.addRect(_leftBorderRect);

  painter->setPen(Qt::NoPen);
  painter->setBrush(_brush);
  painter->drawPath(path);
  painter->setBrush(QBrush(Qt::green));
  painter->drawPath(rightBorderPath);
  painter->setBrush(QBrush(Qt::red));
  painter->drawPath(leftBorderPath);
}

void WavePick::resize(QSizeF scaleC) {
  _size =
      QSizeF(_size.width() * scaleC.width(), _size.height() * scaleC.height());
  _rect.setSize(_size);
  _leftBorderRect.setSize(_size);
  _rightBorderRect.setSize(_size);
}

void WavePick::setLeftFillRect(WaveZone *zone) {
  _leftFillRect = zone;
  updateWaveZone();
}

void WavePick::setRightFillRect(WaveZone *zone) {
  _rightFillRect = zone;
  updateWaveZone();
}

void WavePick::setWaveRect(WaveZone *left, WaveZone *right) {
  _leftFillRect = left;
  _rightFillRect = right;
  updateWaveZone();
}

void WavePick::setAnchor(const QPointF point) { _anchor = point; }

void WavePick::updateGeometry() {
  prepareGeometryChange();
  _rect.moveCenter(QPointF(0, 0));
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

QSizeF WavePick::scallByAxis(QSizeF scaleS) {
  qreal scaleY = scaleS.height();
  _size.setWidth(3); // some optimal const
  _size.setHeight(_size.height() * scaleY);

  _rect.setSize(_size);
  _leftBorderRect.setSize(_size);
  _rightBorderRect.setSize(_size);
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
  //  std::cerr << _chart->mapToValue(mapToParent(event->pos()) -
  //                                  event->buttonDownPos(Qt::LeftButton))
  //                   .x()
  //            << " "
  //            << _chart->mapToValue(mapToParent(event->pos()) -
  //                                  event->buttonDownPos(Qt::LeftButton))
  //                   .y()
  //            << std::endl;
  //  std::cerr << "Pick " << _anchor.x() << " " << _anchor.y() << std::endl;
  //  std::cerr << "LeftBorder" << _leftBorderAnchor.x() << " "
  //            << _leftBorderAnchor.y() << std::endl;
  //  std::cerr << "Right Border " << _rightBorderAnchor.x()
  //            << _rightBorderAnchor.y() << std::endl
  //            << std::endl;
  if (event->buttons() == Qt::RightButton &&
      (Qt::AltModifier == QGuiApplication::keyboardModifiers())) {
    emit needDelete();
  }
  event->setAccepted(true);
}

void WavePick::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton &&
      (Qt::AltModifier == QGuiApplication::keyboardModifiers())) {
    QPointF newPosition = QPointF(_chart->mapToValue(
        mapToParent(event->pos()) /*- event->buttonDownPos(Qt::LeftButton)*/));

    std::cerr << newPosition.x() << " " << _leftBorderAnchor.x() << " "
              << _anchor.x() << " " << _rightBorderAnchor.x() << std::endl;
    if (abs(_anchor.x() -
            _chart->mapToValue(mapToParent(event->pos()) /*-
                               event->buttonDownPos(Qt::LeftButton)*/).x()) <= 0.0005 ||
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

    std::cerr << "after pick " << std::endl;

    if (abs(_leftBorderAnchor.x() -
            _chart->mapToValue(mapToParent(event->pos()) /*-
                               event->buttonDownPos(Qt::LeftButton)*/).x()) <= 0.0005 ||
        NOW_MOVE::L_BORDER == _move) {
      _move = L_BORDER;
      if (newPosition.x() < _anchor.x() && newPosition.x() > 0) {
        setPos(QPointF(
            mapToParent(event->pos() /*- event->buttonDownPos(Qt::LeftButton)*/)
                .x(),
            pos().y()));
        _leftBorderAnchor = QPointF(newPosition.x(), _anchor.y());
        updateGeometry();
      }
      return;
    }

    std::cerr << "after left " << std::endl;

    if (abs(_rightBorderAnchor.x() -
            _chart->mapToValue(mapToParent(event->pos()) /*-
                               event->buttonDownPos(Qt::LeftButton)*/).x()) <= 0.0005 ||
        NOW_MOVE::R_BORDER == _move) {
      _move = R_BORDER;
      if (newPosition.x() > _anchor.x()) {
        setPos(QPointF(
            mapToParent(event->pos() /*- event->buttonDownPos(Qt::LeftButton)*/)
                .x(),
            pos().y()));
        _rightBorderAnchor = QPointF(newPosition.x(), _anchor.y());
        updateGeometry();
      }
    }
    return;
  }
}

void WavePick::updateWaveZone() {

  // wave
  //  if (_rightFillRect != nullptr && _leftFillRect != nullptr) {
  //    _rightFillRect->setWidth(
  //        _chart->mapToPosition(QPointF(_valueRightBorder, 0)).x() -
  //        _chart->mapToPosition(_anchor).x());
  //    _rightFillRect->setAnchor(_anchor);

  //    _leftFillRect->setWidth(
  //        _chart->mapToPosition(_anchor).x() -
  //        _chart->mapToPosition(QPointF(_valueLeftBorder, 0)).x());
  //    return;
  //  }

  //  // right board
  //  if (_leftFillRect != nullptr) {
  //    _leftFillRect->setWidth(
  //        _chart->mapToPosition(_anchor).x() -
  //        _chart->mapToPosition(QPointF(_valueLeftBorder, 0)).x());
  //    return;
  //  }

  //  // left board
  //  if (_rightFillRect != nullptr) {
  //    _rightFillRect->setWidth(
  //        _chart->mapToPosition(QPointF(_valueRightBorder, 0)).x() -
  //        _chart->mapToPosition(_anchor).x());
  //    _rightFillRect->setAnchor(_anchor);
  //    return;
  //  }
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
