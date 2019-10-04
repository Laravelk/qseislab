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

void Controller::addWaveArrival(int index) {
  _view->addPick(QPointF(_pWaveArrival - 500, WAVE_RADIUS + index),
                 QSize(5, 30), Qt::darkRed, _rangeAxisX);
  _view->addPick(QPointF(_sWaveArrival - 500, WAVE_RADIUS + index),
                 QSize(5, 30), Qt::darkBlue, _rangeAxisX);
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
