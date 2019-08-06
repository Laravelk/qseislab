#include "graphicevent.h"
#include "data/seismevent.h"
#include "data/seismcomponent.h"
#include "data/seismtrace.h"

typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;

namespace AddEvent {
GraphicEvent::GraphicEvent(QWidget *parent)
    :QFrame(parent), _chart(new Chart()) {
    _view = new ChartView(_chart);
    _chart->setAnimationOptions(QChart::NoAnimation);
    _chart->legend()->hide();
    _chart->setMinimumSize(750,500);
    _view->hide();
}

void GraphicEvent::update(const std::unique_ptr<SeismEvent> &event) {
    _view->chart()->removeAllSeries();
    setInterval(event);
    for (int i = 0; i < event->getComponentNumber(); i++) {
        SeismComponent *component = event->getComponent(i).get();
        _pWaveArrival = component->getTraces().at(0)->getPWaveArrival();
        _sWaveArrival = component->getTraces().at(0)->getSWaveArrival();
        QLineSeries *seriesPWaveArrival = new QLineSeries;
        QLineSeries *seriesSWaveArrival = new QLineSeries;

        seriesPWaveArrival->append(_pWaveArrival, -_interval / 5);
        seriesPWaveArrival->append(_pWaveArrival, _interval / 5);
        seriesPWaveArrival->setColor(Qt::black);


        seriesSWaveArrival->append(_sWaveArrival, -_interval / 5);
        seriesSWaveArrival->append(_sWaveArrival, _interval / 5);
        seriesSWaveArrival->setColor(Qt::red);

        _chart->addSeries(seriesPWaveArrival);
        _chart->addSeries(seriesSWaveArrival);

        for (int j = 0; j < component->getTraces().size(); j++) {
            float tmp = 0, intervalAxisX = 0;
            _norm = component->getMaxValue();
            QLineSeries *series = new QLineSeries;
            series->setUseOpenGL(true);
            SeismTrace *trace = component->getTraces().at(j).get();
            intervalAxisX = trace->getSampleInterval();
            for (int k = 0; k < trace->getBufferSize(); k++) {
                series->append(tmp, trace->getBuffer()[k] / _norm + _interval * i * 2 + 2 * i);
                tmp+=intervalAxisX;
            }
        _chart->addSeries(series);
        }
    }
    _chart->createDefaultAxes();
    _view->setRenderHint(QPainter::Antialiasing);
    _view->show();
}

void GraphicEvent::clear()
{
        _view->chart()->removeAllSeries();
        _view->hide();
}

void GraphicEvent::setInterval(const std::unique_ptr<SeismEvent> &event)
{
    _interval = 0;
    for (int i = 0; i < event.get()->getComponents().size(); i++) {
        if (_interval < event.get()->getComponent(i).get()->getMaxValue()) {
            _interval = event.get()->getComponent(i).get()->getMaxValue();
        }
    }
}

}


