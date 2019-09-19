#pragma once

#include <QtCharts/QChart>
#include <vector>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

using namespace QtCharts;

class Model : public QChart {
public:
  explicit Model(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = 0);
  ~Model() { waves.clear(); }

  void addWave(QRectF &);

protected:
  bool sceneEvent(QEvent *event);

private:
  bool gestureEvent(QGestureEvent *event);
  std::vector<QRectF> waves;
};
