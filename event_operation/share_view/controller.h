#pragma once

#include "model.h"
#include "view/view.h"

#include <QtCharts>

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
} // namespace Data

namespace EventOperation {
class Controller : public QFrame {
  Q_OBJECT

public:
  explicit Controller(QWidget *parent = nullptr);

  View *getView() const { return _view; }
  Model *getModel() const { return _model; }
  void setChart(Model *chart) { _model = chart; }
  void setView(View *view) { _view = view; }

  void update(const std::unique_ptr<Data::SeismEvent> &event);
  void clear();

private:
  float _norm;
  float _interval;
  int _pWaveArrival;
  int _sWaveArrival;
  View *_view;
  Model *_model;
  QValueAxis *_axisX = new QValueAxis;
  QValueAxis *_axisY = new QValueAxis;

  void setWaveArrivalPen(QLineSeries &pWaveArrivalSeries,
                         QLineSeries &sWaveArrivalSeries);
  void addWaveArrivalSeries(QLineSeries &pWaveArrivalSeries,
                            QLineSeries &sWaveArrivalSeries, int index);
  void setInterval(const std::unique_ptr<Data::SeismEvent> &event);
  void addTraceSeries(const std::unique_ptr<Data::SeismComponent> &component,
                      int index);
  void setAxesY(int componentNumber);

private:
  const qreal AMPLITUDE_SCALAR = 0.5;
  const qreal TRACE_OFFSET = 0.15;
  const qreal WAVE_ARRIVAL_RADIUS = 0.4;
  const float NORMED = 1.7f;
  const int WINDOW_WIDHT = 750;
  const int WINDOW_HEIGHT = 500;
};

} // namespace EventOperation
