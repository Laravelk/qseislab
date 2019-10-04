#include "controller.h"

#include <iostream> // TODO: delete

#include "data/seismcomponent.h"
#include "data/seismevent.h"
#include "data/seismtrace.h"
#include "view/wavepick.h"

typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;

namespace EventOperation {
Controller::Controller(QWidget *parent)
    : QFrame(parent), _model(new Model()), _axisX(new QValueAxis),
      _axisY(new QValueAxis), _rangeAxisX(0) {
  _view = new View(_model);
  _view->addModel(_model);
  _model->setAnimationOptions(QChart::NoAnimation);
  _model->legend()->hide();
  _model->setMinimumSize(GRAPH_WIDHT, GRAPH_HEIGHT);
  _model->addAxis(_axisX, Qt::AlignBottom);
  _model->addAxis(_axisY, Qt::AlignLeft);
  _model->setAcceptHoverEvents(true);
  _view->hide();
}

void Controller::update(const std::unique_ptr<SeismEvent> &event) {
  _view->chart()->removeAllSeries();
  getRangeX(event);
  setInterval(event);
  setAxesY(event->getComponentNumber());
  int idx = 0;
  for (auto &component : event->getComponents()) {
    _pWaveArrival = component->getPWaveArrival();
    _sWaveArrival = component->getSWaveArrival();
    addWaveArrival(idx);
    addTraceSeries(component, idx);
    idx++;
  }
  _model->addPicks(_view->getPickcs());
  _view->show();
}

void Controller::clear() {
  _view->chart()->removeAllSeries();
  _view->hide();
}

void Controller::setWaveArrivalPen(QLineSeries &pWaveArrival,
                                   QLineSeries &sWaveArrival) {
  QColor red, blue;
  QPen pen = pWaveArrival.pen();
  pen.setWidth(5);
  pWaveArrival.setPen(pen);
  sWaveArrival.setPen(pen);
  red.setRed(100);
  blue.setBlue(100);
  sWaveArrival.setColor(red);
  pWaveArrival.setColor(blue);
}

void Controller::setBorderPen(QLineSeries &leftBorder,
                              QLineSeries &rightBorder) {
  QColor green, orange;
  QPen pen = leftBorder.pen();
  pen.setWidth(WAVE_PEN_WIDTH);
  leftBorder.setPen(pen);
  rightBorder.setPen(pen);
  green.setGreen(100);
  orange.setHsl(39, 100, 50); // orange color in hsl format
  leftBorder.setColor(green);
  leftBorder.setColor(orange);
}

void Controller::addWaveArrival(int index) {
  _view->addPick(QPointF(_pWaveArrival - 500, WAVE_RADIUS + index),
                 QSize(20, 40));
}

void Controller::addBorderWavesSeries(QLineSeries &rightBorder,
                                      QLineSeries &leftBorder, int waveCord,
                                      int index) {
  setBorderPen(rightBorder, leftBorder);

  rightBorder.append(static_cast<qreal>(waveCord + _rangeAxisX / 20),
                     BORDER_RADIUS + index);
  rightBorder.append(static_cast<qreal>(waveCord + _rangeAxisX / 20),
                     -BORDER_RADIUS + index);

  leftBorder.append(static_cast<qreal>(waveCord - _rangeAxisX / 20),
                    BORDER_RADIUS + index);
  leftBorder.append(static_cast<qreal>(waveCord - _rangeAxisX / 20),
                    -BORDER_RADIUS + index);

  _model->addSeries(&rightBorder);
  _model->addSeries(&leftBorder);

  rightBorder.attachAxis(_axisX);
  rightBorder.attachAxis(_axisY);
  leftBorder.attachAxis(_axisX);
  leftBorder.attachAxis(_axisY);
}

void Controller::setInterval(const std::unique_ptr<SeismEvent> &event) {
  _interval = 0;
  for (auto &component : event->getComponents()) {
    if (_interval < component->getMaxValue()) {
      _interval = component->getMaxValue();
    }
  }
}

void EventOperation::Controller::addTraceSeries(
    const std::unique_ptr<Data::SeismComponent> &component, int index) {
  const float intervalAxisX = component->getSampleInterval();
  int idx = -1;
  for (unsigned j = 0; j < component->getTraces().size(); ++j, ++idx) {
    _norm = component->getMaxValue() * NORMED;
    QLineSeries *series = new QLineSeries;
    series->setUseOpenGL(true);
    SeismTrace *trace = component->getTraces().at(j).get();
    float tmp = 0;
    for (int k = 0; k < trace->getBufferSize(); k++) {
      series->append(static_cast<qreal>(tmp),
                     TRACE_OFFSET * (idx) +
                         AMPLITUDE_SCALAR * trace->getBuffer()[k] / _norm +
                         index);
      tmp += intervalAxisX;
    }
    _model->addSeries(series);
    series->attachAxis(_axisX);
    series->attachAxis(_axisY);
  }
}

void Controller::setAxesY(int componentNumber) {
  _axisY->setTickCount(componentNumber);
  _axisY->setRange(-1, componentNumber + 0.5);
  _axisY->setTickInterval(1);
  _axisY->setTickAnchor(0);
  _axisY->setTickType(QValueAxis::TicksDynamic);
  _axisY->setLabelFormat("%d");
}

void Controller::getRangeX(const std::unique_ptr<Data::SeismEvent> &event) {
  float sampleInterval = 0;
  int maxCountElementInTrace = 0;
  for (auto &component : event->getComponents()) {
    for (auto &trace : component->getTraces()) {
      if (trace->getBufferSize() > _rangeAxisX) {
        maxCountElementInTrace = trace->getBufferSize();
        sampleInterval = component->getSampleInterval();
      }
    }
  }
  _rangeAxisX = sampleInterval * maxCountElementInTrace;
}

} // namespace EventOperation
