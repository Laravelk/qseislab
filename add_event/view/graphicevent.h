#pragma once

#include <QtCharts>
#include <iostream> // need to delete

#include "chartview.h"
#include "chart.h"

namespace Data {
    class SeismEvent;
    class SeismComponent;
    class SeismTrace;
}

namespace AddEvent {
class GraphicEvent : public QFrame
{
Q_OBJECT

public:
    explicit GraphicEvent(QWidget *parent = nullptr);

    ChartView* getView() const { return _view; }
    Chart* getChart() const { return _chart;}
    void setChart(Chart *chart) { _chart = chart; }
    void setView(ChartView *view) { _view = view; }

    void update(const std::unique_ptr<Data::SeismEvent> &event);
    void clear();

private:
    float _norm;
    float _interval;
    float _pWaveArrival;
    float _sWaveArrival;
    ChartView* _view;
    Chart* _chart;

    void setInterval(const std::unique_ptr<Data::SeismEvent> &event);
};
}

