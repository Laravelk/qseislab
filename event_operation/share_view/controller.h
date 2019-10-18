#pragma once

#include "chartgesture.h"
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
  ChartGesture *getModel() const { return _chart; }
  void setChart(ChartGesture *chart) { _chart = chart; }
  void setView(View *view) { _view = view; }

  void update(const std::unique_ptr<Data::SeismEvent> &);
  void clear();

private:
  float _norm;
  float _interval;
  int _pWaveArrival;
  int _sWaveArrival;
  float _rangeAxisX;
  View *_view;
  ChartGesture *_chart;
  QValueAxis *_axisX = new QValueAxis;
  QValueAxis *_axisY = new QValueAxis;

  void addWaveArrival(int);
  void setInterval(const std::unique_ptr<Data::SeismEvent> &);
  void addTraceSeries(const std::unique_ptr<Data::SeismComponent> &, int);
  void setAxesY(int);
  void getRangeX(const std::unique_ptr<Data::SeismEvent> &);

signals:
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);

private:
  const qreal AMPLITUDE_SCALAR = 0.5;
  const qreal TRACE_OFFSET = 0.25;
  const qreal WAVE_RADIUS = 0.4;
  const qreal BORDER_RADIUS = 0.35;
  const float NORMED = 2.1f;
  const int GRAPH_WIDHT = 750;
  const int GRAPH_HEIGHT = 470;
  const int WAVE_PEN_WIDTH = 4;
  const int BORDER_PEN_WIDTH = 2;
};

} // namespace EventOperation
