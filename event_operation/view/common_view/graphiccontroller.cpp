#include "graphiccontroller.h"

#include "data/seismcomponent.h"
#include "data/seismevent.h"
#include "data/seismtrace.h"
#include "graphic_view/wavepick.h"

#include <iostream>

typedef Data::SeismComponent SeismComponent;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;

namespace EventOperation {
GraphicController::GraphicController(QWidget *parent)
    : QFrame(parent), _rangeAxisX(0), _chart(new ChartGesture()),
      _axisX(new QValueAxis), _axisY(new QValueAxis) {
  _view = new GraphicView(_chart);
  _view->addModel(_chart);
  _view->setWaveRadius(WAVE_RADIUS);
  _chart->setAnimationOptions(QChart::NoAnimation);
  _chart->legend()->hide();
  _chart->setMinimumSize(GRAPH_WIDHT, GRAPH_HEIGHT);
  _chart->addAxis(_axisX, Qt::AlignBottom);
  _chart->addAxis(_axisY, Qt::AlignLeft);
  _chart->setAcceptHoverEvents(true);
  connect(_view, &GraphicView::sendPicksInfo,
          [this](Data::SeismWavePick::Type type, int componentNumber,
                 int leftBorderPos, int pickPos, int rightBorderPos) {
            emit sendPicksInfo(type, componentNumber, leftBorderPos, pickPos,
                               rightBorderPos);
          });
  _view->hide();
}

// подумать, как вызывать удаление, сделать полную очистку
void GraphicController::update(const std::unique_ptr<SeismEvent> &event) {
  _view->chart()->removeAllSeries();
  _view->clearPicks();
  _view->setDefaultScale();
  _rangeAxisX = 0;
  getRangeX(event);
  _view->setCountOfComponents(event->getComponentNumber());
  _view->setRangeX(_rangeAxisX);
  setInterval(event);
  setAxesY(event->getComponentNumber());
  _chart->setReceiverCount(event->getComponentNumber());
  int numberOfComponent = 0;
  for (auto &component : event->getComponents()) {
    for (auto &pick : component->getWavePicks()) {
      addWaveArrival(pick.second, numberOfComponent);
    }
    addTraceSeries(component, numberOfComponent);
    numberOfComponent++;
  }
  _chart->addPicks(_view->getPickcs());
  _view->show();
}

void GraphicController::clear() {
  _view->chart()->removeAllSeries();
  _view->hide();
}

void GraphicController::addWaveArrival(Data::SeismWavePick pick, int index) {
  QSizeF size(2, 40);
  QColor color;
  if (pick.getType() == Data::SeismWavePick::PWAVE) {
    color = Qt::darkRed;
  } else {
    color = Qt::darkBlue;
  }
  _view->addPick(pick.getType(), QPointF(pick.getArrival() - 500, index), size,
                 color, _rangeAxisX, pick.getPolarizationLeftBorder(),
                 pick.getPolarizationRightBorder());
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
        sampleInterval = component->getSampleInterval();
      }
    }
  }
  _rangeAxisX = sampleInterval * maxCountElementInTrace;
}

} // namespace EventOperation
