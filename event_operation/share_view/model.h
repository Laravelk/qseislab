#pragma once

#include <QtCharts/QChart>
#include <vector>

#include "view/graphicswaveitem.h"

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

using namespace QtCharts;

class Model : public QChart {
  Q_OBJECT

public:
  explicit Model(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = 0);

  void addWaves(const QRectF &);
  void addBorders(const QRectF &);
  void addItem(const EventOperation::GraphicsWaveItem *);
  bool isWave(const qreal x, const qreal y) const;

  std::vector<EventOperation::GraphicsWaveItem *> items;

protected:
  bool sceneEvent(QEvent *) override;

private:
  bool gestureEvent(QGestureEvent *event);

private:
  std::vector<QRectF> borders;
  std::vector<QRectF> waves;
};
