#pragma once

#include "chart.h"
#include "chartview.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

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
  ~GraphicEvent();

  ChartView *getView() const { return _view; }
  Chart *getChart() const { return _chart; }
  void setChart(Chart *chart) { _chart = chart; }
  void setView(ChartView *view) { _view = view; }

  void update(const std::unique_ptr<Data::SeismEvent> &event);
  void clear();

private:
  float _norm;
  float _interval;
  float _pWaveArrival;
  float _sWaveArrival;
  ChartView *_view;
  Chart *_chart;
  QValueAxis *_axisX = new QValueAxis;
  QValueAxis *_axisY = new QValueAxis;

  void setWaveArrivalPen(QLineSeries &pWaveArrivalSeries,
                         QLineSeries &sWaveArrivalSeries);
  void addWaveArrivalSeries(QLineSeries &pWaveArrivalSeries,
                            QLineSeries &sWaveArrivalSeries, int index);
  void setInterval(const std::unique_ptr<Data::SeismEvent> &event);
  void addTraceSeries(const Data::SeismComponent *component, int index);
  void setAxesY(int componentNumber);
};

} // namespace EventOperation
