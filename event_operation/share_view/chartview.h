#pragma once

#include <QWheelEvent>
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

using namespace QtCharts;

namespace EventOperation {
class ChartView : public QChartView {
public:
  ChartView(QChart *chart, QWidget *parent = nullptr);

protected:
  bool viewportEvent(QEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  bool m_isTouching;
  qreal _mFactor = 1.0;
};
} // namespace EventOperation
