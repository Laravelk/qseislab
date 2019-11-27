#include "polargraph.h"

#include <data/seismevent.h>
#include <data/seismwavepick.h>

#include <iostream> // TODO: delete

namespace EventOperation {
PolarGraph::PolarGraph(QWidget *parent)
    : QFrame(parent), _polarChart(new QPolarChart()),
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

  _allView = new QWidget();

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addWidget(_polarView, 1);
  _allView->setLayout(mainLayout);
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
        //                std::cerr << polarAngle << " " <<
        //                data->getIncidenceInRadian() << std::endl;
        series->append(polarAngle, data->getIncidenceInRadian());
      }
    }
  }
  _polarChart->addSeries(series);
  series->attachAxis(_radialAxis);
  series->attachAxis(_angularAxis);
}
} // namespace EventOperation
