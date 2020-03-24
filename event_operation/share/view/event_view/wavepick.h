#pragma once

#include "data/seismwavepick.h"

#include <QBrush>
#include <QGuiApplication>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsItem>

#include <variant>
#include <iostream>

QT_BEGIN_NAMESPACE
class QGraphicsSceneEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE;

namespace EventOperation {
class WavePick : public QObject, public QGraphicsItem {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
public:
  WavePick(Data::SeismWavePick::Type, QGraphicsItem *, QChart *, QPointF,
           QSizeF, QBrush, QPointF, QPointF);

  /* Устанавливает якорь для основного pick*/
  void setAnchor(const QPointF);
  void setLeftBorderAnchor(const QPointF);
  void setRightBorderAnchor(const QPointF);

  void setBordersColor(const QColor);
  void setPickColor(const QColor);

  void updateGeometry();

  qreal getXPos() { return _anchor.x(); }
  QSizeF scallByAxis(QSizeF scaleS);
  Data::SeismWavePick::Type getType() { return _type; }
  int getComponentAmount() { return static_cast<int>(_anchor.y()); }

  QRectF boundingRect() const;
  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
  void resize(QSizeF);

signals:
  void changed(float left_board, float pick, float right_border);
  void deleted();

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *);

private:
  enum NOW_MOVE { NONE, WAVE, R_BORDER, L_BORDER };
  const qreal DEFAULT_OFFSET_TO_BORDER = 10000;
  const float DRAG_OFFSET = 0.0007;
  const qreal MAX_WIDTH;
  const QSizeF DEFAULT_SIZE;
  NOW_MOVE _move = NONE;
  Data::SeismWavePick::Type _type;
  QChart *_chart;
  QPointF _pos;
  QSizeF _size;

  QPointF _anchor;
  QRectF _rect;
  QBrush _pickBrush;

  QPointF _leftBorderAnchor;
  QPointF _rightBorderAnchor;
  QRectF _leftBorderRect;
  QRectF _rightBorderRect;
  QBrush _borderBrush = Qt::green;

  QRectF _leftFillRect;
  QRectF _rightFillRect;
  QBrush _fillZoneBrush = QBrush(QColor(100, 100, 100, 100));

private:
  void updateWaveZone();
};
} // namespace EventOperation
