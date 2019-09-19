#pragma once

#include <QtCharts/QChart>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

using namespace QtCharts;

class Model : public QChart {
public:
  explicit Model(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = 0);

protected:
  bool sceneEvent(QEvent *event);

private:
  bool gestureEvent(QGestureEvent *event);
};
