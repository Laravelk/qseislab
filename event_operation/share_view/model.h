#pragma once

#include <QtCharts/QChart>
#include <vector>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

using namespace QtCharts;

class Model : public QChart {
  Q_OBJECT

public:
  explicit Model(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = 0);
  ~Model() = default;

  void addWaves(const QRectF &);
  void addBorders(const QRectF &);

  bool isWave(const qreal x, const qreal y) const;
  void printWaves();

protected:
  bool sceneEvent(QEvent *event);

private:
  bool gestureEvent(QGestureEvent *event);

private:
  std::vector<QRectF> borders;
  std::vector<QRectF> waves;
};
