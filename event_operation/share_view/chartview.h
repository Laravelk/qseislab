#pragma once

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QWheelEvent>


using namespace QtCharts;

class ChartView : public QChartView {
public:
    ChartView(QChart* chart, QWidget* parent = nullptr);

protected:
    bool viewportEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
    bool m_isTouching;
    qreal _mFactor = 1.0;
};
