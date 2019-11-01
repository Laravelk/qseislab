#include "graphiccontroller.h"

#include "data/seismcomponent.h"
#include "data/seismevent.h"
#include "data/seismtrace.h"
#include "graphic_view/wavepick.h"

#include <iostream> // TODO: need to DELETE

typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;

namespace EventOperation {
GraphicController::GraphicController(QWidget *parent)
    : QFrame(parent), _rangeAxisX(0), _chart(new ChartGesture()),
      _axisX(new QValueAxis), _axisY(new QValueAxis) {
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
  _view->hide();
}

void GraphicController::update(const std::unique_ptr<SeismEvent> &event) {
  _event = event.get();
  _view->chart()->removeAllSeries();
  _view->clearPicks();
  _view->setDefaultScale();
  _rangeAxisX = 0;
  getRangeX(event);
  _view->setCountOfComponents(event->getComponentAmount());
  _view->setRangeX(_rangeAxisX);
  setInterval(event);
  setAxesY(event->getComponentAmount());
  _chart->setReceiverCount(event->getComponentAmount());
  int componentAmount = 0;
  for (auto &component : event->getComponents()) {
    for (auto &pick : component->getWavePicks()) {
      addWaveArrival(pick.second, componentAmount);
    }
    addTraceSeries(component, componentAmount);
    componentAmount++;
  }
  _chart->addPicks(_view->getPickcs());
  _view->show();
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
  _gain = 1.0f;
  _clipping = 10.0f;
  _view->hide();
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
  QSizeF size(2, 40);
  QColor color;
  if (pick.getType() == Data::SeismWavePick::PWAVE) {
    color = Qt::darkRed;
  } else {
    color = Qt::darkBlue;
  }
  std::cerr << _rangeAxisX << std::endl;

  _view->addPick(
      pick.getType(),
      QPointF((double)pick.getArrival() / MICROSECONDS_IN_SECOND -
                  (double)500 / MICROSECONDS_IN_SECOND,
              index),
      size, color, _rangeAxisX,
      (double)pick.getPolarizationLeftBorder() / MICROSECONDS_IN_SECOND,
      (double)pick.getPolarizationRightBorder() / MICROSECONDS_IN_SECOND);
}

void GraphicController::setInterval(const std::unique_ptr<SeismEvent> &event) {
  _interval = 0;
  for (auto &component : event->getComponents()) {
    if (_interval < component->getMaxValue()) {
      _interval = component->getMaxValue();
    }
  }
}

void GraphicController::addTraceSeries(
    const std::unique_ptr<Data::SeismComponent> &component, int index) {
  const float intervalAxisX =
      component->getSampleInterval() / MICROSECONDS_IN_SECOND;
  const QColor color[] = {QColor(220, 20, 60), QColor(50, 205, 50),
                          QColor(65, 105, 225)};
  int idx = -1;
  for (unsigned j = 0; j < component->getTraces().size(); ++j, ++idx) {
    _norm = component->getMaxValue() * NORMED;
    QLineSeries *series = new QLineSeries;
    series->setUseOpenGL(true); // NOTE: COMMENT FOR RELEASE
    SeismTrace *trace = component->getTraces().at(j).get();
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
    series->setColor(color[j]);
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
} // namespace EventOperation

void GraphicController::settingAreaSeries(QAreaSeries *series) {
  QPen pen(0x059605);
  pen.setWidth(1);
  series->setPen(pen);
  QBrush brush(Qt::black);
  series->setBrush(brush);
}

void GraphicController::setAxesY(int componentNumber) {
  _axisY->setTickCount(componentNumber);
  _axisY->setRange(-1, componentNumber + 0.5);
  _axisY->setTickInterval(1);
  _axisY->setTickAnchor(0);
  _axisY->setTickType(QValueAxis::TicksDynamic);
  _axisY->setLabelFormat("%d");
}

void GraphicController::getRangeX(
    const std::unique_ptr<Data::SeismEvent> &event) {
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
  if (_clipping < _gain) {
    currentGain = _clipping;
  }
  for (auto &component : _event->getComponents()) {
    _norm = component->getMaxValue() / currentGain * NORMED;
    int index = -1;
    for (auto &trace : component->getTraces()) {
      QList<QPointF> points;
      QList<QPointF> oldPoints = seriesIterator.i->t()->points();
      if ((index == -1 && !_hideAxisX) || (index == 0 && !_hideAxisY) ||
          (index == 1 && !_hideAxisZ)) {
        for (int k = 0; k < trace->getBufferSize(); k++) {
          float newYValue = trace->getBuffer()[k] * currentGain / _norm;
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
