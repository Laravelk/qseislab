#pragma once

#include <QtCharts/QChart>
#include <vector>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

using namespace QtCharts;

namespace EventOperation {
class Model : public QChart {
  Q_OBJECT

public:
  explicit Model(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = 0);

protected:
  bool sceneEvent(QEvent *) override;

private:
  bool gestureEvent(QGestureEvent *event);
public slots:

private:
};
} // namespace EventOperation
