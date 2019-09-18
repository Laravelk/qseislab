#include "graphicevent.h"

#include "data/seismcomponent.h"
#include "data/seismevent.h"
#include "data/seismtrace.h"

typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;

namespace EventOperation {
GraphicEvent::GraphicEvent(QWidget *parent)
    : QFrame(parent), _chart(new Chart()), _axisX(new QValueAxis),
      _axisY(new QValueAxis) {
  _view = new ChartView(_chart);
  _chart->setAnimationOptions(QChart::NoAnimation);
  _chart->legend()->hide();
  _chart->setMinimumSize(WINDOW_WIDHT, WINDOW_HEIGHT);
  _chart->addAxis(_axisX, Qt::AlignBottom);
  _chart->addAxis(_axisY, Qt::AlignLeft);
  _view->hide();
}

void GraphicEvent::update(const std::unique_ptr<SeismEvent> &event) {
  _view->chart()->removeAllSeries();
  setInterval(event);
  setAxesY(event->getComponentNumber());
  int idx = 0;
  for (auto &component : event->getComponents()) {
    _pWaveArrival = component->getPWaveArrival();
    _sWaveArrival = component->getSWaveArrival();
    QLineSeries *seriesPWaveArrival = new QLineSeries;
    QLineSeries *seriesSWaveArrival = new QLineSeries;
    setWaveArrivalPen(*seriesPWaveArrival, *seriesSWaveArrival);
    addWaveArrivalSeries(*seriesPWaveArrival, *seriesSWaveArrival, idx);
    addTraceSeries(component, idx);
    ++idx;
  }
  _view->setRenderHint(QPainter::Antialiasing);
  _view->show();
}

void GraphicEvent::clear() {
  _view->chart()->removeAllSeries();
  _view->hide();
}

void GraphicEvent::setWaveArrivalPen(QLineSeries &pWaveArrivalSeries,
                                     QLineSeries &sWaveArrivalSeries) {
  QColor red, blue;
  QPen pen = pWaveArrivalSeries.pen();
  pen.setWidth(5);
  pen.setBrush(QBrush("red"));
  pWaveArrivalSeries.setPen(pen);
  sWaveArrivalSeries.setPen(pen);
  red.setRed(100);
  blue.setBlue(100);
  sWaveArrivalSeries.setColor(red);
  pWaveArrivalSeries.setColor(blue);
}

void GraphicEvent::addWaveArrivalSeries(QLineSeries &pWaveArrivalSeries,
                                        QLineSeries &sWaveArrivalSeries,
                                        int index) {
  pWaveArrivalSeries.append(_pWaveArrival, WAVE_ARRIVAL_RADIUS + index);
  pWaveArrivalSeries.append(_pWaveArrival, -WAVE_ARRIVAL_RADIUS + index);

  sWaveArrivalSeries.append(_sWaveArrival, WAVE_ARRIVAL_RADIUS + index);
  sWaveArrivalSeries.append(_sWaveArrival, -WAVE_ARRIVAL_RADIUS + index);

  _chart->addSeries(&pWaveArrivalSeries);
  _chart->addSeries(&sWaveArrivalSeries);

  pWaveArrivalSeries.attachAxis(_axisX);
  pWaveArrivalSeries.attachAxis(_axisY);

  sWaveArrivalSeries.attachAxis(_axisX);
  sWaveArrivalSeries.attachAxis(_axisY);
}

void GraphicEvent::setInterval(const std::unique_ptr<SeismEvent> &event) {
  _interval = 0;
  for (auto &component : event->getComponents()) {
    if (_interval < component->getMaxValue()) {
      _interval = component->getMaxValue();
    }
  }
}

void EventOperation::GraphicEvent::addTraceSeries(
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
    _chart->addSeries(series);
    series->attachAxis(_axisX);
    series->attachAxis(_axisY);
  }
}

void GraphicEvent::setAxesY(int componentNumber) {
  _axisY->setTickCount(componentNumber);
  _axisY->setRange(-1, componentNumber + 0.5);
  _axisY->setTickInterval(1);
  _axisY->setTickAnchor(0);
  _axisY->setTickType(QValueAxis::TicksDynamic);
  _axisY->setLabelFormat("%d");
}

} // namespace EventOperation
