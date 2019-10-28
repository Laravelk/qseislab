#pragma once

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
                        Qt::WindowFlags wFlags = 0);
  void addPicks(QList<WavePick *> *wavePicks) { _wavePicks = wavePicks; }
  void setReceiverCount(int count) { receiverCount = count; }
  int  getReceiverCount() { return receiverCount; }
  QList<WavePick *> *getWavePicks() {return  _wavePicks;}
protected:
  bool sceneEvent(QEvent *) override;
private:
  bool gestureEvent(QGestureEvent *event);
private:
  QList<WavePick *> *_wavePicks;
  int receiverCount = 0;
};
} // namespace EventOperation
