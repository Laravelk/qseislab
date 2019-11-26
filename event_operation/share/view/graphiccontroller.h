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
<<<<<<< HEAD

  //  QWidget *getView() const { return _allView; }
  ChartGesture *getModel() const { return _chart; }
  void setChart(ChartGesture *chart) { _chart = chart; }
  void setView(GraphicView *view) { _view = view; }

  void update(Data::SeismEvent const *const);
=======
  QWidget *getView() const { return _allView; }
  ChartGesture *getModel() const { return _chart; }
  void setChart(ChartGesture *chart) { _chart = chart; }
  void update(const std::unique_ptr<Data::SeismEvent> &);
>>>>>>> test
  void updateEventName(const QString &);
  void setGainCoefficient(const float gainCoefficient);
  void setClippingValue(const float clippingValue);
  void setWiggle(const int status);
<<<<<<< HEAD
=======
  void setAddPolarizationWindowButtonEnable(bool enable) {
          _polarizationEventButton->setEnabled(enable);
  }
>>>>>>> test
  void clear();

  void hideAxisX(bool);
  void hideAxisY(bool);
  void hideAxisZ(bool);

private:
<<<<<<< HEAD
  //  QWidget *_allView;

  Data::SeismEvent const *_event;
=======
  QWidget *_allView;
  GraphicView *_view;
  Data::SeismEvent *_event;
>>>>>>> test
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
<<<<<<< HEAD
  GraphicView *_view;
=======
>>>>>>> test
  ChartGesture *_chart;
  QValueAxis *_axisX = new QValueAxis;
  QValueAxis *_axisY = new QValueAxis;
  QList<QAreaSeries *> _positiveWiggleSeries;
  QList<QAreaSeries *> _negativeWiggleSeries;

  QList<QLineSeries *> _allSeries;

  void addWaveArrival(Data::SeismWavePick, int);
<<<<<<< HEAD
  void setInterval(Data::SeismEvent const *const);
  void addTraceSeries(const std::shared_ptr<Data::SeismComponent> &, int);
  void addWiggle(bool t); // true is positive, false is negative
  void settingAreaSeries(QAreaSeries *series);
  void setAxesY(int);
  void getRangeX(Data::SeismEvent const *const);
=======
  void setInterval(const std::unique_ptr<Data::SeismEvent> &);
  void addTraceSeries(const std::unique_ptr<Data::SeismComponent> &, int);
  void addWiggle(bool t); // true is positive, false is negative
  void settingAreaSeries(QAreaSeries *series);
  void setAxesY(int);
  void getRangeX(const std::unique_ptr<Data::SeismEvent> &);
>>>>>>> test
  void updateSeries();
  double findPointAroundZero(int, int, QPointF &, QPointF &);
  void deleteAllWiggle();

<<<<<<< HEAD
signals:
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void createPolarizationAnalysisWindowClicked();
=======

signals:
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void removePick(Data::SeismWavePick::Type, int);
  void createPolarizationAnalysisWindowClicked();
  void calculatePolarizationAnalysisDataClicked();
>>>>>>> test

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

<<<<<<< HEAD
  //  void settingGraphicMenu();
  //  void showGraphicMenu();
  //  void hideGraphicMenu();
  //  void settingWiggleButton();
  //  void insertRowInComponentsHideTable(const QString &, int);

=======
>>>>>>> test
  WiggleWidget *_wiggleWidget;
  HideComponentWidget *_hideComponentWidget;
  ClippingWidget *_clippingWidget;
  GainWidget *_gainWidget;

<<<<<<< HEAD
  //  QSlider *_clippingSlider;
  //  QSlider *_gainSlider;
  //  QLabel *_clippintSliderLabel;
  //  QLabel *_gainSliderLabel;

  //  QTableWidget *_hideComponentsTable;

=======
>>>>>>> test
  QPushButton *_addWaveButton;
  QAction *_addPWave;
  QAction *_addSWave;

  QPushButton *_polarizationEventButton;
<<<<<<< HEAD

  //  QRadioButton *_noneWiggle;
  //  QRadioButton *_positiveWiggle;
  //  QRadioButton *_negativeWiggle;
=======
  QPushButton *_calculatePolarizationAnalysisDataButton;
>>>>>>> test
};

} // namespace EventOperation
