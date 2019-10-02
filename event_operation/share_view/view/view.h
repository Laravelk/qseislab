#pragma once

#include "../model.h"
#include "wavepick.h"
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE

namespace EventOperation {
class View : public QChartView {
  Q_OBJECT
public:
  View(QChart *, QWidget *parent = nullptr);
  void addModel(Model *model) { _chart = model; }
  void addPick(WavePick *pick);
  void addPick(); // test

  QList<WavePick *> getPickcs() { return wavePicks; }

protected:
  bool viewportEvent(QEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;
  void mouseDoubleClickEvent(QMouseEvent *) override;
  void paintEvent(QPaintEvent *) override;
  void scrollContentsBy(int dx, int dy) override;
  void resizeEvent(QResizeEvent *) override;
  void scaleContentsBy(qreal factor);

private:
  bool mouseIsTouching = false;
  qreal _mFactor = 1.0;
  Model *_chart;
  QList<WavePick *> wavePicks;

private:
};
} // namespace EventOperation
