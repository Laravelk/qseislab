#pragma once

#include "../model.h"

#include <QWheelEvent>
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
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

private:
  bool mouseIsTouching = false;
  bool altIsTouching = false;
  qreal _mFactor = 1.0;
  Model *_model;

private:
  bool isWave();
  bool isBoard();
};
