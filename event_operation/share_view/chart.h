#pragma once

#include <QtCharts/QChart>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

using namespace QtCharts;

namespace EventOperation {
class Chart : public QChart {
public:
  explicit Chart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = 0);

protected:
  bool sceneEvent(QEvent *event);

private:
  bool gestureEvent(QGestureEvent *event);
};
} // namespace EventOperation
