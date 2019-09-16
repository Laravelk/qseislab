#pragma once

#include "chart.h"
#include "chartview.h"

#include <QtCharts>

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
} // namespace Data

namespace EventOperation {
class GraphicEvent : public QFrame {
  Q_OBJECT

public:
  explicit GraphicEvent(QWidget *parent = nullptr);

  ChartView *getView() const { return _view; }
  Chart *getChart() const { return _chart; }
  void setChart(Chart *chart) { _chart = chart; }
  void setView(ChartView *view) { _view = view; }

  void update(const std::unique_ptr<Data::SeismEvent> &event);
  void clear();

private:
  int _pWaveArrival;
  int _sWaveArrival;
  float _interval;
  float _norm;
  ChartView *_view;
  Chart *_chart;
  QValueAxis *_axisX = new QValueAxis;
  QValueAxis *_axisY = new QValueAxis;

private:
  void setWaveArrivalPen(QLineSeries &pWaveArrivalSeries,
                         QLineSeries &sWaveArrivalSeries);
  void addWaveArrivalSeries(QLineSeries &pWaveArrivalSeries,
                            QLineSeries &sWaveArrivalSeries, int index);
  void setInterval(const std::unique_ptr<Data::SeismEvent> &event);
  void addTraceSeries(const std::unique_ptr<Data::SeismComponent> &component,
                      int index);
  void setAxesY(int componentNumber);

private:
  const qreal _AMPLITUDE_SCALAR = 0.5;
  const qreal _TRACE_OFFSET = 0.15;
  const qreal _WAVE_ARRIVAL_RADIUS = 0.4;
  const float _NORMED = 1.7f;
};

} // namespace EventOperation
