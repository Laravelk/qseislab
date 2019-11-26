#pragma once

#include <QtCharts/QChart>
#include <vector>
#include <QDebug>

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
                        Qt::WindowFlags wFlags = 0);
  void addPicks(QList<WavePick *> *wavePicks) { _wavePicks = wavePicks; }
  void setReceiverCount(int count) { receiverCount = count; }
  int getReceiverCount() { return receiverCount; }

  QList<WavePick *> *getWavePicks() { return _wavePicks; }
<<<<<<< HEAD:event_operation/share/view/chartgesture.h
=======
  void zoom(qreal);
  void zoomIn(const QRectF &rect);
>>>>>>> test:event_operation/view/common_view/chartgesture.h

protected:
  bool sceneEvent(QEvent *) override;

private:
  bool gestureEvent(QGestureEvent *event);

private:
  QList<WavePick *> *_wavePicks;
  int receiverCount = 0;
};
} // namespace EventOperation
