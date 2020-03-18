#pragma once

#include "data/seismwavepick.h"
#include "wavezone.h"

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
  void setAnchor(const QPointF);

  void updateGeometry();

  qreal getXPos() { return _anchor.x(); }
  QSizeF scallByAxis(QSizeF scaleS);
  Data::SeismWavePick::Type getType() { return _type; }
  int getComponentAmount() { return static_cast<int>(_anchor.y()); }

  QRectF boundingRect() const;
  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
  void resize(QSizeF);
  void emitChanged() { emit changed(); }

  /* Это стоит вызывать только у PWAVE && SWAVE. Не у границ */
  void setLeftFillRect(WaveZone *);
  void setRightFillRect(WaveZone *);
  void setWaveRect(WaveZone *, WaveZone *);

signals:
  void changed();
  void needDelete();

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *);

private:
  const qreal DEFAULT_OFFSET_TO_BORDER = 10000;
  const qreal MAX_WIDTH;
  const QSizeF DEFAULT_SIZE;
  bool _wasChanged = false;
  Data::SeismWavePick::Type _type;
  QChart *_chart;
  QPointF _pos;
  QSizeF _size;
  QPointF _anchor;
  QPointF _leftBorderAnchor;
  QPointF _rightBorderAnchor;
  QRectF _rect;
  QRectF _leftBorderRect;
  QRectF _rightBorderRect;
  QBrush _brush;
  WaveZone *_rightFillRect = nullptr;
  WaveZone *_leftFillRect = nullptr;

private:
  void updateWaveZone();
};
} // namespace EventOperation
