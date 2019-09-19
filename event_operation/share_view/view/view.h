#pragma once

#include <QWheelEvent>
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

using namespace QtCharts;

class View : public QChartView {
public:
  View(QChart *, QWidget *parent = nullptr);

  void addWaveRect(QRect);

protected:
  bool viewportEvent(QEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void keyPressEvent(QKeyEvent *);
  void wheelEvent(QWheelEvent *);
  void keyReleaseEvent(QKeyEvent *);

private:
  bool mouseIsTouching = false;
  bool altIsTouching = false;
  qreal _mFactor = 1.0;

  std::vector<QRect> _qRectVector;
};
