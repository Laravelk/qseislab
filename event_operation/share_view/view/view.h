#pragma once

#include "../model.h"

#include <QWheelEvent>
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QtWidgets>
#include <vector>

using namespace QtCharts;

class View : public QChartView {
  Q_OBJECT
public:
  View(QChart *, QWidget *parent = nullptr);
  ~View() {}

  void addModel(Model *model) { _model = model; }

protected:
  bool viewportEvent(QEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void keyPressEvent(QKeyEvent *);
  void wheelEvent(QWheelEvent *);
  void keyReleaseEvent(QKeyEvent *);
  void mouseDoubleClickEvent(QMouseEvent *);

private:
  bool mouseIsTouching = false;
  bool altIsTouching = false;
  bool waveIsLock = false;
  bool borderIsLock = false;
  qreal _mFactor = 1.0;
  Model *_model;
  int x = 1000;

private:
  bool isWave();
  bool isBoard();
};
