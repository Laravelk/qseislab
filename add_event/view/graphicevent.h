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


        ChartView* getView() { return _view; }
        Chart* getChart() { return _chart;}

        void SetChart(Chart *chart) { _chart = chart; }
        void SetView(ChartView *view) { _view = view; }
        void update(const std::unique_ptr<Data::SeismEvent> &event);
        void clear();

    private:
        const float _INTERVAL = 2.6e+10; // TODO: потом возьмем максимальное значение события
        float cdpX;
        float cdpY;
        ChartView* _view;
        Chart* _chart;
        QValueAxis* _axisX;
        QValueAxis* _axisY;

    };
}

