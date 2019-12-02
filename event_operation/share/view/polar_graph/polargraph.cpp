#include "polargraph.h"

#include <data/seismevent.h>
#include <data/seismwavepick.h>

#include <iostream> // TODO: delete

namespace EventOperation {
PolarGraph::PolarGraph(QWidget *parent)
    : QWidget(parent), _polarChart(new QPolarChart()),
      _polarView(new QChartView()), _angularAxis(new QValueAxis()),
      _radialAxis(new QValueAxis) {
  const qreal angularMin = 0;
  const qreal angularMax = 360;

  const qreal radialMin = 0;
  const qreal radialMax = 3;

  _angularAxis->setTickCount(9);
  _angularAxis->setLabelFormat("%.1f");
  _angularAxis->setShadesBrush(QBrush(QColor(249, 249, 250)));
  _polarChart->addAxis(_angularAxis, QPolarChart::PolarOrientationAngular);

  _radialAxis->setTickCount(9);
  _radialAxis->setLabelFormat("%.1f");
  _polarChart->addAxis(_radialAxis, QPolarChart::PolarOrientationRadial);

  _radialAxis->setRange(radialMin, radialMax);
  _angularAxis->setRange(angularMin, angularMax);

  _polarView->setChart(_polarChart);
  _polarView->setRenderHint(QPainter::Antialiasing);

//  _status = new QLabel(WARNING_STATUS);
//  _status->move(20, 450);
  _statusRect = new QGraphicsRectItem(20, 442, 133, 20, _polarChart);
  _statusRect->setBrush(Qt::yellow);
  _status = new QGraphicsTextItem(WARNING_STATUS, _polarChart);
  _status->setPos(20, 440);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addWidget(_polarView);
  setLayout(mainLayout);
}

QWidget *PolarGraph::getView() const { return _allView; }

void PolarGraph::update(Data::SeismEvent const * const event) {
  _polarChart->removeAllSeries();
  QScatterSeries *series = new QScatterSeries();
  for (auto &component : event->getComponents()) {
    for (auto &pick : component->getWavePicks()) {
      if (pick.second.getPolarizationAnalysisData() != std::nullopt) {
        Data::SeismPolarizationAnalysisData *data =
            pick.second.getPolarizationAnalysisData().value();
        double polarAngle =
            std::fmod(data->getAzimutDegrees(), 360) > 0
                ? std::fmod(data->getAzimutDegrees(), 360)
                : 360 + std::fmod(data->getAzimutDegrees(), 360);
        series->append(polarAngle, data->getIncidenceInRadian());
      }
    }
  }
  connect(series, &QScatterSeries::pressed, [](const QPointF &point){
      std::cerr << "pressed"  << point.x() << " " << point.y();
  });
  _polarChart->addSeries(series);
  _seriesList.append(series);
  series->attachAxis(_radialAxis);
  series->attachAxis(_angularAxis);
}

void PolarGraph::setGraphColor(const QBrush &color)
{
//    _polarView->setAutoFillBackground(true);
//    _polarChart->setBackgroundBrush(color);
//    _polarChart->setBackgroundVisible(true);
}

void PolarGraph::setScatterColor(const QBrush &color)
{
    for (auto &series : _seriesList) {
        series->setColor(color.color());
    }
}

void PolarGraph::setAlarmAboutUnvalidData(bool alarmStatus)
{
    if (alarmStatus) {
        _status->show();
        setScatterColor(Qt::gray);
    } else {
        _status->hide();
        setScatterColor(Qt::blue);
    }
}
} // namespace EventOperation
















