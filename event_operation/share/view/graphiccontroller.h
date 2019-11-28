#pragma once

#include "chartgesture.h"
#include "data/seismevent.h"
#include "graphic_view/graphic_view.h"
#include "polar_graph/polargraph.h"

#include <QtCharts>

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
} // namespace Data

namespace EventOperation {
class WiggleWidget;
class HideComponentWidget;
class ClippingWidget;
class GainWidget;
class GraphicController : public QFrame {
  Q_OBJECT
public:
  explicit GraphicController(QWidget *parent = nullptr);

  //  QWidget *getView() const { return _allView; }
  ChartGesture *getModel() const { return _chart; }
  void setChart(ChartGesture *chart) { _chart = chart; }
  void setView(GraphicView *view) { _view = view; }

  void update(Data::SeismEvent const *const);
  void updatePolarGraph(Data::SeismEvent const *const);

  void updateEventName(const QString &);
  void setGainCoefficient(const float gainCoefficient);
  void setClippingValue(const float clippingValue);
  void setWiggle(const int status);
  void showWarningAboutUnvalidDataOnGraph(bool);

  void setAddPolarizationWindowButtonEnable(bool enable) {
    _polarizationEventButton->setEnabled(enable);
  }
  void clear();

  void hideAxisX(bool);
  void hideAxisY(bool);
  void hideAxisZ(bool);

private:
  //  QWidget *_allView;
  GraphicView *_view;
  PolarGraph *_polarGraph;
  QTabWidget *_tabWidget;
  Data::SeismEvent const *_event;

  float _norm;
  float _interval;
  int _pWaveArrival;
  int _sWaveArrival;
  float _rangeAxisX;
  float _clipping = 10.0f;
  float _gain = 1.0f;
  bool _hideAxisX = false;
  bool _hideAxisY = false;
  bool _hideAxisZ = false;
  bool _isPositiveWiggleSet = false;
  bool _isNegativeWiggleSet = false;

  ChartGesture *_chart;
  QValueAxis *_axisX = new QValueAxis;
  QValueAxis *_axisY = new QValueAxis;
  QList<QAreaSeries *> _positiveWiggleSeries;
  QList<QAreaSeries *> _negativeWiggleSeries;

  QList<QLineSeries *> _allSeries;

  void addWaveArrival(Data::SeismWavePick, int);
  void setInterval(Data::SeismEvent const *const);
  void addTraceSeries(Data::SeismComponent const *const, int);
  void addWiggle(bool t); // true is positive, false is negative
  void settingAreaSeries(QAreaSeries *series);
  void setAxesY(int);
  void getRangeX(Data::SeismEvent const *const);

  void updateSeries();
  double findPointAroundZero(int, int, QPointF &, QPointF &);
  void deleteAllWiggle();

signals:
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void removePick(Data::SeismWavePick::Type, int);
  void createPolarizationAnalysisWindowClicked();
  void calculatePolarizationAnalysisDataClicked();
  void clickOnPolarAnalysisInGraph();

private:
  const qreal AMPLITUDE_SCALAR = 0.1;
  const qreal TRACE_OFFSET = 0.25;
  const float NORMED = 2.1f;
  const int GRAPH_WIDHT = 750;
  const int GRAPH_HEIGHT = 470;
  const int MICROSECONDS_IN_SECOND = 1000000;
  const int MICROSECONDS_IN_MILISECOND = 1000;

private:
  void showToolsMenu();
  void hideToolsMenu();

  WiggleWidget *_wiggleWidget;
  HideComponentWidget *_hideComponentWidget;
  ClippingWidget *_clippingWidget;
  GainWidget *_gainWidget;

  QPushButton *_addWaveButton;
  QAction *_addPWave;
  QAction *_addSWave;

  QPushButton *_polarizationEventButton;
  QPushButton *_calculatePolarizationAnalysisDataButton;

  const int GRAPH_INDEX_IN_TAB = 0;
  const int POLAR_ANALYSIS_INDEX_IN_TAB = 1;
};

} // namespace EventOperation
