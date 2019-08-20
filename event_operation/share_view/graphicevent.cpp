#include "graphicevent.h"

#include "data/seismevent.h"
#include "data/seismcomponent.h"
#include "data/seismtrace.h"


typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;


namespace EventOperation {
GraphicEvent::GraphicEvent(QWidget* parent)
    :QFrame(parent),
     _chart(new Chart()),
     _axisX(new QValueAxis),
     _axisY(new QValueAxis)
{
    _view = new ChartView(_chart);
    _chart->setAnimationOptions(QChart::NoAnimation);
    _chart->legend()->hide();
    _chart->setMinimumSize(750,500);
    _chart->addAxis(_axisX, Qt::AlignBottom);
    _chart->addAxis(_axisY, Qt::AlignLeft);
    _view->hide();
}

void GraphicEvent::update(const std::unique_ptr<SeismEvent>& event)
{
    _view->chart()->removeAllSeries();
    setInterval(event);
    setAxesY(event->getComponentNumber());
    for (int i = 0; i < event->getComponentNumber(); i++) {
        SeismComponent *component = event->getComponent(i).get();
        _pWaveArrival = component->getPWaveArrival();
        _sWaveArrival = component->getSWaveArrival();
        QLineSeries *seriesPWaveArrival = new QLineSeries;
        QLineSeries *seriesSWaveArrival = new QLineSeries;
        setWaveArrivalPen(*seriesPWaveArrival, *seriesSWaveArrival);
        addWaveArrivalSeries(*seriesPWaveArrival, *seriesSWaveArrival, i);
        addTraceSeries(component, i);
    }
//    _chart->createDefaultAxes();
    _view->setRenderHint(QPainter::Antialiasing);
    _view->show();
    }

void GraphicEvent::clear()
{
        _view->chart()->removeAllSeries();
        _view->hide();
}

void GraphicEvent::setWaveArrivalPen(QLineSeries& pWaveArrivalSeries, QLineSeries& sWaveArrivalSeries)
{
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

void GraphicEvent::addWaveArrivalSeries(QLineSeries& pWaveArrivalSeries, QLineSeries& sWaveArrivalSeries, int index)
{
    pWaveArrivalSeries.append(_pWaveArrival, 0.8 + index );
    pWaveArrivalSeries.append(_pWaveArrival, -0.8 + index );

    sWaveArrivalSeries.append(_sWaveArrival, 0.8 + index );
    sWaveArrivalSeries.append(_sWaveArrival, -0.8 + index );

    _chart->addSeries(&pWaveArrivalSeries);
    _chart->addSeries(&sWaveArrivalSeries);

    pWaveArrivalSeries.attachAxis(_axisX);
    pWaveArrivalSeries.attachAxis(_axisY);

    sWaveArrivalSeries.attachAxis(_axisX);
    sWaveArrivalSeries.attachAxis(_axisY);
}

void GraphicEvent::setInterval(const std::unique_ptr<SeismEvent>& event)
{
    _interval = 0;
    for (int i = 0; i < event->getComponents().size(); i++) {
        if (_interval < event->getComponent(i)->getMaxValue()) {
            _interval = event->getComponent(i)->getMaxValue();
        }
    }
}

void GraphicEvent::addTraceSeries(const Data::SeismComponent* component, int index)
{
    for (int j = 0; j < component->getTraces().size(); j++) {
        float tmp = 0, intervalAxisX = 0;
        _norm = component->getMaxValue();
//        std::cout << _norm << std::endl;
        QLineSeries *series = new QLineSeries;
        series->setUseOpenGL(true);
        SeismTrace *trace = component->getTraces().at(j).get();
        intervalAxisX = trace->getSampleInterval();
        for (int k = 0; k < trace->getBufferSize(); k++) {
            //std::cout << trace->getBuffer()[k] / _norm << std::endl;
            series->append(tmp, 0.5 * trace->getBuffer()[k] / _norm + index);
            tmp+=intervalAxisX;
        }
        _chart->addSeries(series);
        series->attachAxis(_axisX);
        series->attachAxis(_axisY);
    }

}

void GraphicEvent::setAxesY(int componentNumber)
{
    _axisY->setTickCount(componentNumber);
    _axisY->setRange(-1,componentNumber + 0.5);
    _axisY->setTickInterval(1);
    _axisY->setTickAnchor(0);
    _axisY->setTickType(QValueAxis::TicksDynamic);
    _axisY->setLabelFormat("%d");
}


} // namespace EventOperation
