#pragma once

#include "chartgesture.h"
#include "data/seismevent.h"
#include "graphic_view/graphic_view.h"

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

  QWidget *getView() const { return _allView; }
  ChartGesture *getModel() const { return _chart; }
  void setChart(ChartGesture *chart) { _chart = chart; }
  void setView(GraphicView *view) { _view = view; }

  void update(Data::SeismEvent const *const);
  void updateEventName(const QString &);
  void setGainCoefficient(const float gainCoefficient);
  void setClippingValue(const float clippingValue);
  void setWiggle(const int status);
  void clear();

  void hideAxisX(bool);
  void hideAxisY(bool);
  void hideAxisZ(bool);

private:
  QWidget *_allView;

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
  GraphicView *_view;
  ChartGesture *_chart;
  QValueAxis *_axisX = new QValueAxis;
  QValueAxis *_axisY = new QValueAxis;
  QList<QAreaSeries *> _positiveWiggleSeries;
  QList<QAreaSeries *> _negativeWiggleSeries;

  QList<QLineSeries *> _allSeries;

  void addWaveArrival(Data::SeismWavePick, int);
  void setInterval(Data::SeismEvent const *const);
  void addTraceSeries(const std::shared_ptr<Data::SeismComponent> &, int);
  void addWiggle(bool t); // true is positive, false is negative
  void settingAreaSeries(QAreaSeries *series);
  void setAxesY(int);
  void getRangeX(Data::SeismEvent const *const);
  void updateSeries();
  double findPointAroundZero(int, int, QPointF &, QPointF &);
  void deleteAllWiggle();

signals:
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void createPolarizationAnalysisWindowClicked();

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

  //  void settingGraphicMenu();
  //  void showGraphicMenu();
  //  void hideGraphicMenu();
  //  void settingWiggleButton();
  //  void insertRowInComponentsHideTable(const QString &, int);

  WiggleWidget *_wiggleWidget;
  HideComponentWidget *_hideComponentWidget;
  ClippingWidget *_clippingWidget;
  GainWidget *_gainWidget;

  //  QSlider *_clippingSlider;
  //  QSlider *_gainSlider;
  //  QLabel *_clippintSliderLabel;
  //  QLabel *_gainSliderLabel;

  //  QTableWidget *_hideComponentsTable;

  QPushButton *_addWaveButton;
  QAction *_addPWave;
  QAction *_addSWave;

  QPushButton *_polarizationEventButton;

  //  QRadioButton *_noneWiggle;
  //  QRadioButton *_positiveWiggle;
  //  QRadioButton *_negativeWiggle;
};

} // namespace EventOperation
