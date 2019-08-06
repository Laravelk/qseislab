#include "graphicevent.h"
#include "data/seismevent.h"
#include "data/seismcomponent.h"
#include "data/seismtrace.h"

namespace AddEvent {
GraphicEvent::GraphicEvent(QWidget *parent)
    :QFrame(parent), _chart(new Chart()) {
    _view = new ChartView(_chart);
    _chart->setAnimationOptions(QChart::SeriesAnimations);
    _chart->legend()->hide();
    _chart->createDefaultAxes();
    _chart->setMinimumSize(750,500);


    _view->setRenderHint(QPainter::Antialiasing);
    _view->hide();
}

void GraphicEvent::update(const std::unique_ptr<Data::SeismEvent> &event) {
    _view->chart()->removeAllSeries();
    for (int i = 0; i < event->getComponentNumber(); i++) {
                QLineSeries *seriesX = new QLineSeries;
                QLineSeries *seriesY = new QLineSeries;
                QLineSeries *seriesZ = new QLineSeries;
                QLineSeries *seriesCdpX = new QLineSeries;
                QLineSeries *seriesCdpY = new QLineSeries;
                Data::SeismComponent *component = event->getComponent(i).get();
                Data::SeismTrace *traceX = component->getTraceX().get();
                Data::SeismTrace *traceY = component->getTraceY().get();
                Data::SeismTrace *traceZ = component->getTraceZ().get();
                double intervalX = traceX->getSampleInterval();
                double intervalY = traceY->getSampleInterval();
                double intervalZ = traceZ->getSampleInterval();
                cdpX = traceX->getCdpX();
                cdpY = traceY->getCdpY();

                seriesCdpX->append(cdpX, 0.5f + i * _INTERVAL); // max
                seriesCdpX->append(cdpX,-0.5f - i * _INTERVAL); // min

                seriesCdpY->append(cdpY, 0.5f + i * _INTERVAL); // max
                seriesCdpY->append(cdpY,-0.5f - i * _INTERVAL); // min

                double tmp = 0;
                for (int j = 0; j < traceX->getBufferSize(); j++) {
                    seriesX->append(tmp,traceX->getBuffer().get()[j] + i*_INTERVAL);
                    if (traceX->getBuffer().get()[j] > 4 | traceX->getBuffer().get()[j] < 0.4)
//                        std::cout << tmp << " TRACEX " << traceX->getBuffer().get()[j] << "\n";
                    tmp+=intervalX;
                }
                tmp = 0;
                for (int j = 0; j < traceY->getBufferSize(); j++) {
                    seriesY->append(tmp, traceY->getBuffer().get()[j] + i*_INTERVAL);
                    if (traceY->getBuffer().get()[j] > 4 | traceY->getBuffer().get()[j] < 0.4)
                    tmp+=intervalY;
                }
                tmp = 0;
                for (int j = 0; j < traceZ->getBufferSize(); j++) {
                    seriesZ->append(tmp, traceZ->getBuffer().get()[j] + i*_INTERVAL);
                    tmp+=intervalZ;
                }
                _chart->addSeries(seriesX);
                _chart->addSeries(seriesY);
                _chart->addSeries(seriesZ);
            }
            _chart->createDefaultAxes();
            _chart->axisX()->setRange(0,500); // max x
            _view->setRenderHint(QPainter::Antialiasing);
            _view->show();
}

void GraphicEvent::clear() {
    _view->chart()->removeAllSeries();
    _view->hide();
}




