#pragma once

#include "chartgesture.h"
#include "graphic_view/graphic_view.h"
#include "data/seismevent.h"

#include <QtCharts>

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
} // namespace Data

namespace EventOperation {
class GraphicController : public QFrame {
  Q_OBJECT
public:
  explicit GraphicController(QWidget *parent = nullptr);

  GraphicView *getView() const { return _view; }
  ChartGesture *getModel() const { return _chart; }
  void setChart(ChartGesture *chart) { _chart = chart; }
  void setView(GraphicView *view) { _view = view; }

  void update(const std::unique_ptr<Data::SeismEvent> &);
  void setGainCoefficient(const float gainCoefficient);
  void setClippingValue(const float clippingValue);
  void clear();

private:
  Data::SeismEvent *_event;
  float _norm;
  float _interval;
  int _pWaveArrival;
  int _sWaveArrival;
  float _rangeAxisX;
  float _clipping = 1000000.0f;
  float _gain = 1.0f;
  GraphicView *_view;
  ChartGesture *_chart;
  QValueAxis *_axisX = new QValueAxis;
  QValueAxis *_axisY = new QValueAxis;

  QList<QLineSeries *> _allSeries;

  void addWaveArrival(Data::SeismWavePick, int);
  void setInterval(const std::unique_ptr<Data::SeismEvent> &);
  void addTraceSeries(const std::unique_ptr<Data::SeismComponent> &, int);
  void setAxesY(int);
  void getRangeX(const std::unique_ptr<Data::SeismEvent> &);
  void updateSeriesByGain(const float gainCoefficient);
  void updateSeriesByClipping(const float clippingValue);
  void updateSeries();

signals:
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);

private:
  const qreal AMPLITUDE_SCALAR = 0.5;
  const qreal TRACE_OFFSET = 0.25;
  const float NORMED = 2.1f;
  const int GRAPH_WIDHT = 750;
  const int GRAPH_HEIGHT = 470;
};

} // namespace EventOperation
