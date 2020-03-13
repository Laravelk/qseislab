#include "graphiccontroller.h"

#include "data/seismcomponent.h"
#include "data/seismevent.h"
#include "data/seismtrace.h"
#include "event_view/wavepick.h"

#include "tools_view/changeborderonpolarwidget.h"
#include "tools_view/clippingwidget.h"
#include "tools_view/gainwidget.h"
#include "tools_view/hidecomponentwidget.h"
#include "tools_view/hidewavepointswidget.h"
#include "tools_view/wigglewidget.h"

#include <iostream> // TODO: need to DELETE

typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;

namespace EventOperation {
GraphicController::GraphicController(QWidget *parent)
    : QFrame(parent), _rangeAxisX(0), _chart(new ChartGesture()),
      _axisX(new QValueAxis), _axisY(new QValueAxis),
      _wiggleWidget(new WiggleWidget()),
      _hideComponentWidget(new HideComponentWidget()),
      _clippingWidget(new ClippingWidget()), _gainWidget(new GainWidget()),
      _addWaveButton(new QPushButton("+")),
      _polarizationEventButton(new QPushButton("Hodogram")),
      _calculatePolarizationAnalysisDataButton(new QPushButton("Compute")),
      _polarChart(new QPolarChart()),
      _hideWavePointsWidget(new HideWavePointsWidget()),
      _changeBorderWidget(new ChangeBorderOnPolarWidget()),
      _testButton(new QPushButton("Test")), _screenButton(new QPushButton()) {

  _view = new GraphicView(_chart);
  _view->addModel(_chart);
  _chart->setAnimationOptions(QChart::NoAnimation);
  _chart->legend()->hide();
  _chart->setMinimumSize(GRAPH_WIDHT, GRAPH_HEIGHT);
  _chart->addAxis(_axisX, Qt::AlignBottom);
  _chart->addAxis(_axisY, Qt::AlignLeft);
  _chart->setAcceptHoverEvents(true);
  connect(_view, &GraphicView::sendPicksInfo,
          [this](Data::SeismWavePick::Type type, int componentAmount,
                 int leftBorderPos, int pickPos, int rightBorderPos) {
            emit sendPicksInfo(type, componentAmount, leftBorderPos, pickPos,
                               rightBorderPos);
          });

  connect(_view, &GraphicView::addPickSignal,
          [this](auto type, auto num, auto l_val, auto arrival, auto r_val) {
            emit addPick(type, num, l_val, arrival, r_val);
          });

  connect(_view, &GraphicView::removePick,
          [this](Data::SeismWavePick::Type type, int componentAmount) {
            emit removePick(type, componentAmount);
          });

  _calculatePolarizationAnalysisDataButton->setMinimumSize(135, 20);

  QMenu *addWaveButtonMenu = new QMenu(_addWaveButton);
  addWaveButtonMenu->setMinimumSize(135, 20);
  _addPWave = new QAction("PWAVE", _addWaveButton);
  _addSWave = new QAction("SWAVE", _addWaveButton);
  addWaveButtonMenu->addAction(_addPWave);
  addWaveButtonMenu->addAction(_addSWave);
  _addWaveButton->setMenu(addWaveButtonMenu);

  _screenButton->setText("Screenshot");

  // conect`s

  connect(_addPWave, &QAction::triggered, [this]() {
    _view->setWaveAddTriggerFlag(Data::SeismWavePick::PWAVE);
  });

  connect(_addSWave, &QAction::triggered, [this]() {
    _view->setWaveAddTriggerFlag(Data::SeismWavePick::SWAVE);
  });

  connect(_wiggleWidget, &WiggleWidget::updateWiggleState,
          [this](WiggleWidget::WiggleState state) {
            deleteAllWiggle();
            switch (state) {
            case WiggleWidget::WiggleState::Positive:
              _isPositiveWiggleSet = true;
              addWiggle(true);
              break;
            case WiggleWidget::WiggleState::Negative:
              _isNegativeWiggleSet = true;
              addWiggle(false);
              break;
            default:
              break;
            }
          });

  connect(_hideComponentWidget, &HideComponentWidget::updateAxisState,
          [this](auto axis, auto state) {
            switch (axis) {
            case HideComponentWidget::Axis::X:
              hideAxisX(state == HideComponentWidget::State::Unchecked);
              break;
            case HideComponentWidget::Axis::Y:
              hideAxisY(state == HideComponentWidget::State::Unchecked);
              break;
            case HideComponentWidget::Axis::Z:
              hideAxisZ(state == HideComponentWidget::State::Unchecked);
              break;
            }
          });

  connect(_clippingWidget, &ClippingWidget::updateClipping,
          [this](float clipping) {
            _clipping = clipping;
            updateSeries();
          });

  connect(_gainWidget, &GainWidget::updateGain, [this](float gain) {
    _gain = gain;
    updateSeries();
  });

  connect(_testButton, &QPushButton::clicked,
          [this]() { emit createAnalysisWindowTestClicked(); });

  connect(_screenButton, &QPushButton::clicked, [this]() {
    QString format = "png";
    QString initialPath = _event->getName() + "_traces." + format;
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save As"), initialPath,
        tr("%1 Files (*.%2);;All Files (*)").arg(format.toUpper()).arg(format));
    QPixmap pixmap = QPixmap::grabWidget(_view);
    pixmap.save(fileName, format.toUtf8());
  });

  // layout`s

  QVBoxLayout *editGraphicMenuLayout = new QVBoxLayout();
  editGraphicMenuLayout->addWidget(_wiggleWidget);
  editGraphicMenuLayout->addWidget(_hideComponentWidget);
  editGraphicMenuLayout->addWidget(_clippingWidget);
  editGraphicMenuLayout->addWidget(_gainWidget);
  editGraphicMenuLayout->addWidget(_addWaveButton);
  editGraphicMenuLayout->addWidget(_testButton);
  editGraphicMenuLayout->addWidget(_screenButton);
  editGraphicMenuLayout->addStretch(1);

  QHBoxLayout *graphLayout = new QHBoxLayout();
  graphLayout->addWidget(_view, 1);
  graphLayout->addLayout(editGraphicMenuLayout);

  QWidget *graphWidget = new QWidget();
  graphWidget->setLayout(graphLayout);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addWidget(graphWidget, 1);
  setLayout(mainLayout);
}

void GraphicController::update(SeismEvent const *const event) {
  bool isAnotherEvent = false;
  if (_event != event) {
    isAnotherEvent = true;
  }
  _event = event;
  _view->chart()->removeAllSeries();
  _view->clearView();
  _allSeries.clear();
  if (isAnotherEvent) {
    setInterval(event);
    setAxesY(event->getComponentAmount());
    _chart->setReceiverCount(event->getComponentAmount());
    _rangeAxisX = 0;
    getRangeX(event);
    _view->setCountOfComponents(event->getComponentAmount());
    _view->setRangeX(_rangeAxisX);
    _chart->axisX()->setMin(0);
    _chart->axisX()->setMax(_rangeAxisX);
    _view->resetItemSize();
  }
  int componentAmount = 0;
  for (auto &component : event->getComponents()) {
    for (auto &pick : component->getWavePicks()) {
      addWaveArrival(pick.second, componentAmount);
    }
    addTraceSeries(component, componentAmount);
    componentAmount++;
  }
  updateSeries();
}

void GraphicController::updateEventName(const QString &name) {
  _view->chart()->setTitle(name);
  _polarChart->setTitle(name);
}

void GraphicController::setGainCoefficient(const float gainCoefficient) {
  _gain = gainCoefficient;
  updateSeries();
}

void GraphicController::setClippingValue(const float clippingValue) {
  _clipping = clippingValue;
  updateSeries();
}

void GraphicController::deleteAllWiggle() {
  for (auto &wiggleAreaSeries : _positiveWiggleSeries) {
    _chart->removeSeries(wiggleAreaSeries);
  }
  for (auto &wiggleAreaSeries : _negativeWiggleSeries) {
    _chart->removeSeries(wiggleAreaSeries);
  }
  _positiveWiggleSeries.clear();
  _negativeWiggleSeries.clear();
  _isPositiveWiggleSet = false;
  _isNegativeWiggleSet = false;
}

void GraphicController::setWiggle(const int status) {
  if (status == 0) {
    deleteAllWiggle();
  }

  if (status == 1) {
    deleteAllWiggle();
    _isPositiveWiggleSet = true;
    addWiggle(true);
  }

  if (status == 2) {
    deleteAllWiggle();
    _isNegativeWiggleSet = true;
    addWiggle(false);
  }
}

void GraphicController::clear() {
  _view->chart()->removeAllSeries();
  _positiveWiggleSeries.clear();
  _negativeWiggleSeries.clear();
  _hideAxisX = false;
  _hideAxisY = false;
  _hideAxisZ = false;
  _event = nullptr;
}

void GraphicController::hideAxisX(bool hide) {
  _hideAxisX = hide;
  updateSeries();
}

void GraphicController::hideAxisY(bool hide) {
  _hideAxisY = hide;
  updateSeries();
}

void GraphicController::hideAxisZ(bool hide) {
  _hideAxisZ = hide;
  updateSeries();
}

void GraphicController::addWaveArrival(Data::SeismWavePick pick, int index) {
  _view->addPick(
      pick.getType(),
      QPointF(static_cast<double>(pick.getArrival()) / MICROSECONDS_IN_SECOND,
              index),
      _rangeAxisX,
      static_cast<double>(pick.getPolarizationLeftBorder()) /
          MICROSECONDS_IN_SECOND,
      static_cast<double>(pick.getPolarizationRightBorder()) /
          MICROSECONDS_IN_SECOND);
}

void GraphicController::setInterval(SeismEvent const *const event) {
  _interval = 0;
  for (auto &component : event->getComponents()) {
    if (_interval < component->getMaxValue()) {
      _interval = component->getMaxValue();
    }
  }
}

void GraphicController::addTraceSeries(
    Data::SeismComponent const *const component, int index) {
  const float intervalAxisX =
      component->getSampleInterval() / MICROSECONDS_IN_SECOND;
  int idx = -1;
  for (unsigned j = 0; j < component->getTraces().size(); ++j, ++idx) {
    _norm = component->getMaxValue() /* NORMED*/;
    QLineSeries *series = new QLineSeries;
    series->setUseOpenGL(true); // NOTE: COMMENT FOR RELEASE
    auto trace = component->getTraces().at(j);
    float tmp = 0;
    for (int k = 0; k < trace->getBufferSize(); k++) {
      series->append(
          static_cast<qreal>(tmp),
          TRACE_OFFSET * (idx) +
              AMPLITUDE_SCALAR * trace->getBuffer()[k] * _gain / _norm + index);
      tmp += intervalAxisX;
    }
    _chart->addSeries(series);
    connect(series, &QLineSeries::clicked,
            [this](const QPointF &pos) { _view->mouseEvent(pos); });
    series->setColor(_view->getAxisColor(j));
    series->attachAxis(_axisX);
    series->attachAxis(_axisY);
    _allSeries.push_back(series);
  }
}

double GraphicController::findPointAroundZero(int numberOfComponent, int idx,
                                              QPointF &from, QPointF &to) {
  double k = (to.y() - from.y()) / (to.x() - from.x());
  double b = (to.x() * from.y() - to.y() * from.x()) / (to.x() - from.x());
  return (static_cast<double>(TRACE_OFFSET * (idx) + numberOfComponent - b) /
          k);
}

void GraphicController::addWiggle(bool flag) {
  QList<QAreaSeries *> *areaSeries;
  if (flag == true) {
    areaSeries = &_positiveWiggleSeries;
  } else {
    areaSeries = &_negativeWiggleSeries;
  }

  int idx = -1;
  int numberOfComponent = 0;
  for (auto series : _allSeries) {
    double seriesYZeroCord = TRACE_OFFSET * (idx) + numberOfComponent;
    QLineSeries *medianSeries = new QLineSeries();
    medianSeries->setUseOpenGL(true);
    medianSeries->append(0, seriesYZeroCord);
    medianSeries->append(_rangeAxisX, seriesYZeroCord);
    QLineSeries *newSeries = new QLineSeries;
    newSeries->setUseOpenGL(true);
    QPointF lp = series->at(0);
    if (flag == true) {
      for (auto &point : series->points()) {
        double x = point.x();
        if (lp.y() < seriesYZeroCord && point.y() > seriesYZeroCord) {
          x = findPointAroundZero(numberOfComponent, idx, lp, point);
          newSeries->append(QPointF(x, seriesYZeroCord));
        }
        if (lp.y() > seriesYZeroCord && point.y() < seriesYZeroCord &&
            point.y()) {
          x = findPointAroundZero(numberOfComponent, idx, lp, point);
          newSeries->append(x, seriesYZeroCord);
        }
        if (point.y() >= seriesYZeroCord) {
          newSeries->append(point);
        }
        lp = point;
      }
    } else {
      for (auto &point : series->points()) {
        double x = 0;
        if (lp.y() > seriesYZeroCord && point.y() < seriesYZeroCord) {
          x = findPointAroundZero(numberOfComponent, idx, lp, point);
          newSeries->append(QPointF(x, seriesYZeroCord));
        }
        if (lp.y() < seriesYZeroCord && point.y() > seriesYZeroCord) {
          x = findPointAroundZero(numberOfComponent, idx, lp, point);
          newSeries->append(x, seriesYZeroCord);
        }
        if (point.y() <= seriesYZeroCord) {
          newSeries->append(point);
        }
        lp = point;
      }
    }
    QAreaSeries *upperArea = new QAreaSeries();
    QColor upperAreaColor = series->color();
    upperAreaColor.setAlpha(100);
    upperArea->setPen(upperAreaColor);
    upperArea->setBrush(QBrush(upperAreaColor));
    upperArea->setUpperSeries(newSeries);
    upperArea->setLowerSeries(medianSeries);
    upperArea->setUseOpenGL(true);
    settingAreaSeries(upperArea);
    _chart->addSeries(upperArea);
    upperArea->attachAxis(_axisX);
    upperArea->attachAxis(_axisY);
    areaSeries->append(upperArea);
    if ((idx == -1 && _hideAxisX) || (idx == 0 && _hideAxisY) ||
        (idx == 1 && _hideAxisZ)) {
      upperArea->hide();
    }
    idx++;
    if (idx == 2) {
      numberOfComponent++;
      idx = -1;
    }
  }
}

void GraphicController::settingAreaSeries(QAreaSeries *series) {
  QPen pen(0x059605);
  pen.setWidth(1);
  series->setPen(pen);
  series->setBorderColor(Qt::white);
}

void GraphicController::setAxesY(int componentNumber) {
  _axisY->setTickCount(componentNumber);
  _axisY->setRange(-1, componentNumber + 0.5);
  _axisY->setTickInterval(1);
  _axisY->setTickAnchor(0);
  _axisY->setTickType(QValueAxis::TicksDynamic);
  _axisY->setLabelFormat("%d");
}

void GraphicController::getRangeX(SeismEvent const *const event) {
  float sampleInterval = 0;
  int maxCountElementInTrace = 0;
  for (auto &component : event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      if (trace->getBufferSize() > _rangeAxisX) {
        maxCountElementInTrace = trace->getBufferSize();
        sampleInterval =
            component->getSampleInterval() / MICROSECONDS_IN_SECOND;
      }
    }
  }
  _rangeAxisX = sampleInterval * maxCountElementInTrace;
}

void GraphicController::updateSeries() {
  QList<QLineSeries *>::iterator seriesIterator = _allSeries.begin();
  int componentNumber = 0;
  float currentGain = _gain;
  for (auto &component : _event->getComponents()) {
    _norm = component->getMaxValue();
    int index = -1;
    for (auto &trace : component->getTraces()) {
      QList<QPointF> points;
      QList<QPointF> oldPoints = seriesIterator.i->t()->points();
      if ((index == -1 && !_hideAxisX) || (index == 0 && !_hideAxisY) ||
          (index == 1 && !_hideAxisZ)) {
        for (int k = 0; k < trace->getBufferSize(); k++) {
          float newYValue = trace->getBuffer()[k] * currentGain / _norm;
          if (_clipping < abs(newYValue)) {
            newYValue = _clipping * newYValue / abs(newYValue);
          }
          points.append(
              QPointF(oldPoints.at(k).x(), TRACE_OFFSET * (index) +
                                               AMPLITUDE_SCALAR * newYValue +
                                               componentNumber));
        }
        seriesIterator.i->t()->replace(points);
        seriesIterator.i->t()->show();
      } else {
        seriesIterator.i->t()->hide();
      }
      seriesIterator++;
      index++;
    }
    componentNumber++;
  }

  if (_isPositiveWiggleSet) {
    deleteAllWiggle();
    setWiggle(1);
  }
  if (_isNegativeWiggleSet) {
    deleteAllWiggle();
    setWiggle(2);
  }
}

} // namespace EventOperation
