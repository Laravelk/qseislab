#include "polargraph.h"

#include <data/seismevent.h>

#include <limits>
#include <iostream> // TODO: delete

namespace EventOperation {
PolarGraph::PolarGraph(QWidget *parent)
    : QWidget(parent), _polarChart(new QPolarChart()),
      _polarView(new QChartView()), _angularAxis(new QValueAxis()),
      _radialAxis(new QValueAxis), _rect(new QGraphicsRectItem) {
  const qreal angularMin = 0;
  const qreal angularMax = 360;

  const qreal radialMin = 0;
  const qreal radialMax = 3;

  _angularAxis->setTickCount(9);
  _angularAxis->setLabelFormat("%.1f");
  _angularAxis->setShadesBrush(QBrush(QColor(249, 249, 250)));
  _polarChart->addAxis(_angularAxis, QPolarChart::PolarOrientationAngular);
  _polarChart->legend()->hide();

  _radialAxis->setTickCount(9);
  _radialAxis->setLabelFormat("%.1f");
  _polarChart->addAxis(_radialAxis, QPolarChart::PolarOrientationRadial);

  _radialAxis->setRange(radialMin, radialMax);
  _angularAxis->setRange(angularMin, angularMax);

  _polarView->setChart(_polarChart);
  _polarView->setRenderHint(QPainter::Antialiasing);

//  _statusRect = new QGraphicsRectItem(20, 442, 133, 20, _polarChart);
  _statusRect = new QGraphicsRectItem(260, 200, 133, 20, _polarChart);
  _statusRect->setZValue(11);
  _statusRect->setBrush(Qt::yellow);
  _status = new QGraphicsTextItem(WARNING_STATUS, _polarChart);
//  _status->setPos(20, 440);
  _status->setPos(260,198);
  _status->setZValue(12);

  _polarChart->scene()->addItem(_rect);
  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addWidget(_polarView);
  setLayout(mainLayout);
}

QWidget *PolarGraph::getView() const { return _allView; }

void PolarGraph::update(Data::SeismEvent const * const event) {
  _polarChart->removeAllSeries();
  _seriesList.clear();
  _dataList.clear();
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
        _dataList.append(*(pick.second.getPolarizationAnalysisData().value()));
      }
    }
  }
  connect(series, &QScatterSeries::pressed, [this](const QPointF &point){
      for (auto &data : _dataList) {
          qreal dataPolarAngle = static_cast<qreal>(std::fmod(data.getAzimutDegrees(), 360) > 0
                                   ? std::fmod(data.getAzimutDegrees(), 360)
                                   : 360 + std::fmod(data.getAzimutDegrees(), 360));
          qreal dataIncidenceInRadian = static_cast<qreal>(data.getIncidenceInRadian());
            if ((std::fabs(dataPolarAngle - point.x()) < std::numeric_limits<qreal>::epsilon()) &&
                    (std::fabs(dataIncidenceInRadian - point.y()) < std::numeric_limits<qreal>::epsilon())) {
                if (_dataItem == nullptr) {
                    _dataItem = new AnalysisDataGraphicsItem(_polarChart);
                    _dataItem->setZValue(999999);
                }
                _dataItem->setAnchor(point);
                _dataItem->setText("hello");
                _dataItem->updateGeometry();
            }
      }
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
















