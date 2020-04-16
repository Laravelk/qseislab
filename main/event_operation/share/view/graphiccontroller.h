#pragma once

#include <QPainter>
#include <QPixmap>

#include "chartgesture.h"
#include "data/seismevent.h"
#include "event_view/analysis_view/analysiswindow.h"
#include "event_view/graphic_view.h"

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
class HideWavePointsWidget;
class ChangeBorderOnPolarWidget;
class GraphicController : public QFrame {
  Q_OBJECT
public:
  explicit GraphicController(QWidget *parent = nullptr);

  ChartGesture *getModel() const { return _chart; }
  void setChart(ChartGesture *chart) { _chart = chart; }
  void setView(GraphicView *view) { _view = view; }

  void update(Data::SeismEvent const *const);

  void updateEventName(const QString &);
  void setGainCoefficient(const float gainCoefficient);
  void setClippingValue(const float clippingValue);
  void setWiggle(const int status);

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
  Data::SeismEvent const *_event;

  float _norm;
  float _interval;
  int _pWaveArrival;
  int _sWaveArrival;
  float _rangeAxisX;
  float _clipping = 1.0f;
  float _gain = 1.0f;
  bool _hideAxisX = false;
  bool _hideAxisY = false;
  bool _hideAxisZ = false;
  bool _isPositiveWiggleSet = false;
  bool _isNegativeWiggleSet = false;

  ChartGesture *_chart;
  QPolarChart *_polarChart;
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
  void addPick(Data::SeismWavePick::Type, int, int, int, int);
  void createAnalysisWindowClicked();

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
  HideWavePointsWidget *_hideWavePointsWidget;
  ClippingWidget *_clippingWidget;
  GainWidget *_gainWidget;
  ChangeBorderOnPolarWidget *_changeBorderWidget;

  QPushButton *_addWaveButton;
  QAction *_addPWave;
  QAction *_addSWave;

  QPushButton *_polarizationEventButton;
  QPushButton *_calculatePolarizationAnalysisDataButton;
  QPushButton *_analysisButton;
  QPushButton *_screenButton;

  const int GRAPH_INDEX_IN_TAB = 0;
  const int POLAR_ANALYSIS_INDEX_IN_TAB = 1;
};

} // namespace EventOperation