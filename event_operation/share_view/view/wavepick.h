#pragma once

#include "data/seismwavepick.h"

#include <QBrush>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsItem>

QT_BEGIN_NAMESPACE
class QGraphicsSceneEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE;

namespace EventOperation {

// enum PickType { LEFT_BORDER, RIGHT_BORDER, PWAVE, SWAVE };

class WavePick : public QObject, public QGraphicsItem {
  Q_OBJECT
public:
  WavePick(QChart *, QPointF, QSize, QBrush, std::variant<WavePick *, qreal>,
           std::variant<WavePick *, qreal>);
  WavePick(QChart *, qreal, qreal, int, int, QBrush, WavePick *);

  enum WaveType { LEFT_BOARD, RIGHT_BOARD, PICK };

  void setAnchor(const QPointF);
  void updateGeomety();
  qreal getXPos() { return _anchor.x(); }
  void setLeftBorder(std::variant<WavePick *, qreal>);
  void setRightBorder(std::variant<WavePick *, qreal>);
  void setBorders(std::variant<WavePick *, qreal>,
                  std::variant<WavePick *, qreal>);

  QRectF boundingRect() const;
  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

signals:
  void sendTypeNumCompY(Data::SeismWavePick::Type, int, int);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

private:
  const qreal DEFAULT_OFFSET_TO_BORDER = 10000;
  QChart *_chart;
  QPointF _pos;
  QSize _size;
  QPointF _anchor;
  std::variant<WavePick *, qreal> _leftBorder;
  std::variant<WavePick *, qreal> _rightBorder;
  qreal _valueLeftBorder;
  qreal _valueRightBorder;
  QRectF _rect;
  QBrush _brush;

private:
  void updateBorders();
};
} // namespace EventOperation
