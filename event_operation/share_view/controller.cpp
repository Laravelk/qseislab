#include "controller.h"

#include "data/seismcomponent.h"
#include "data/seismevent.h"
#include "data/seismtrace.h"
#include "view/wavepick.h"

typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;

namespace EventOperation {
Controller::Controller(QWidget *parent)
    : QFrame(parent), _chart(new ChartGesture()), _axisX(new QValueAxis),
      _axisY(new QValueAxis), _rangeAxisX(0) {
  _view = new View(_chart);
  _view->addModel(_chart);
  _view->setWaveRadius(WAVE_RADIUS);
  _chart->setAnimationOptions(QChart::NoAnimation);
  _chart->legend()->hide();
  _chart->setMinimumSize(GRAPH_WIDHT, GRAPH_HEIGHT);
  _chart->addAxis(_axisX, Qt::AlignBottom);
  _chart->addAxis(_axisY, Qt::AlignLeft);
  _chart->setAcceptHoverEvents(true);
  connect(_view, &View::sendTypeNumCompY,
          [this](auto type, auto num, auto newPos) {
            emit sendTypeNumCompY(type, num, newPos);
          });
  _view->hide();
}

void Controller::addPick() { _view->setAddPickFlag(true); }

// подумать, как вызывать удаление, сделать полную очистку
void Controller::update(const std::unique_ptr<SeismEvent> &event) {
  _view->chart()->removeAllSeries();
  _view->clearPicks();
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
  _chart->addPicks(_view->getPickcs());
  _view->show();
}

void Controller::clear() {
  _view->chart()->removeAllSeries();
  _view->hide();
}

void Controller::addWaveArrival(int index) {
  _view->addPick(QPointF(_pWaveArrival - 500, WAVE_RADIUS + index),
                 QSize(5, 40), Qt::darkRed, _rangeAxisX);
  _view->addPick(QPointF(_sWaveArrival - 500, WAVE_RADIUS + index),
                 QSize(5, 40), Qt::darkBlue, _rangeAxisX);
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
      series->append(static_cast<qreal>(tmp),
                     TRACE_OFFSET * (idx) +
                         AMPLITUDE_SCALAR * trace->getBuffer()[k] / _norm +
                         index);
      tmp += intervalAxisX;
    }
    _chart->addSeries(series);
    series->setColor(color[j]);
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
