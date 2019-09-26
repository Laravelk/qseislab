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
  void addModel(Model *model) { _model = model; }

protected:
  bool viewportEvent(QEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void wheelEvent(QWheelEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;
  void mouseDoubleClickEvent(QMouseEvent *) override;

private:
  bool mouseIsTouching = false;
  bool altIsTouching = false;
  bool waveIsLock = false;
  bool borderIsLock = false;
  qreal _mFactor = 1.0;
  Model *_model;

private:
  bool isWave();
  bool isBoard();

public slots:
  void f(QRectF &);
};
