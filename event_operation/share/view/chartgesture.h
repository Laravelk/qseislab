#pragma once

#include <QDebug>
#include <QtCharts/QChart>
#include <vector>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

using namespace QtCharts;

namespace EventOperation {
class WavePick;
class ChartGesture : public QChart {
  Q_OBJECT
public:
  explicit ChartGesture(QGraphicsItem *parent = nullptr,
                        Qt::WindowFlags wFlags = nullptr);
  void setReceiverCount(int count) { receiverCount = count; }
  int getReceiverCount() { return receiverCount; }
  void zoom(qreal);
  void zoomIn(const QRectF &rect);

protected:
  bool sceneEvent(QEvent *) override;

private:
  bool gestureEvent(QGestureEvent *event);

private:
  int receiverCount = 0;
};
} // namespace EventOperation
