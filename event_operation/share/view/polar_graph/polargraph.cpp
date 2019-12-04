#include "polargraph.h"

#include <data/seismevent.h>

#include <limits>
#include <iostream> // TODO: delete

namespace EventOperation {
PolarGraph::PolarGraph(QPolarChart *chart,QWidget *parent)
    : QChartView(chart), _polarChart(chart),
       _angularAxis(new QValueAxis()),
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

  this->setRenderHint(QPainter::Antialiasing);

  //  _statusRect = new QGraphicsRectItem(20, 442, 133, 20, _polarChart);
    _statusRect = new QGraphicsRectItem(WARNING_STATUS_RECT, _polarChart);
    _statusRect->setZValue(11);
    _statusRect->setBrush(Qt::yellow);
    _status = new QGraphicsTextItem(WARNING_STATUS, _polarChart);
  //  _status->setPos(20, 440);
    _status->setPos(QPointF(20, 405));
    _status->setZValue(12);

//    _polarChart->scene()->addItem(_rect);
}

QWidget *PolarGraph::getView() const { return _allView; }

void PolarGraph::update(Data::SeismEvent const * const event) {
  _polarChart->removeAllSeries();
  _seriesList.clear();
  _dataList.clear();
  QScatterSeries *validSeries = new QScatterSeries();
  QScatterSeries *unValidSeries = new QScatterSeries();
  unValidSeries->setColor(Qt::gray);
  for (auto &component : event->getComponents()) {
    for (auto &pick : component->getWavePicks()) {
      if (((Data::SeismWavePick::Type::PWAVE == pick.first) && _hidePWave)
              || ((Data::SeismWavePick::Type::SWAVE == pick.first) && _hideSWave)) {
          continue;
      }
      if (pick.second.getPolarizationAnalysisData() != std::nullopt) {
        Data::SeismPolarizationAnalysisData *data =
            pick.second.getPolarizationAnalysisData().value();
        double polarAngle =
            std::fmod(data->getAzimutDegrees(), 360) > 0
                ? std::fmod(data->getAzimutDegrees(), 360)
                : 360 + std::fmod(data->getAzimutDegrees(), 360);
        if (data->isValid()) {
            validSeries->append(polarAngle, data->getIncidenceInRadian());
        }
        else {
           unValidSeries->append(polarAngle, data->getIncidenceInRadian());
        }
        _dataList.append(*(pick.second.getPolarizationAnalysisData().value()));
      }
    }
  }
  _seriesList.append(validSeries);
  connect(validSeries, &QScatterSeries::clicked, [this](const QPointF &point) {
      handleClickedPoint(point);
  });

  connect(unValidSeries, &QScatterSeries::clicked, [this](const QPointF &point) {
      handleClickedPoint(point);
  });

  _status->setPlainText(NORMAL_STATUS);
  _statusRect->hide();
  _statusRect->setBrush(Qt::white);

  if (0 != unValidSeries->points().size()) {
      _status->show();
      _statusRect->setBrush(Qt::yellow);
      _statusRect->show();
      _status->setPlainText(WARNING_STATUS);
      _polarChart->addSeries(unValidSeries);
      unValidSeries->attachAxis(_radialAxis);
      unValidSeries->attachAxis(_angularAxis);
      _seriesList.append(unValidSeries);
  }
  _polarChart->addSeries(validSeries);
  _seriesList.append(validSeries);
  validSeries->attachAxis(_radialAxis);
  validSeries->attachAxis(_angularAxis);

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

void PolarGraph::hideSWavePoints(bool hide)
{
    _hideSWave = hide;
}

void PolarGraph::hidePWavePoints(bool hide)
{
    _hidePWave = hide;
}

void PolarGraph::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Alt: {
      _altIsTouch = true;
      _status->setPlainText(ALT_IS_TOUCHING_STATUS);
      break;
    }
    default:
      QGraphicsView::keyPressEvent(event);
      break;
    }
}

void PolarGraph::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Alt:
      _altIsTouch = false;
      _status->setPlainText(NORMAL_STATUS);
      break;
    default:
      QChartView::keyReleaseEvent(event);
      break;
    }
}

void PolarGraph::handleClickedPoint(const QPointF &point)
{
    QPointF clickedPoint = point;
    QPointF closest(INT_MAX, INT_MAX);
    qreal distance(INT_MAX);
    for (auto &series : _seriesList) {
        auto points = series->points();
        for (const QPointF &currentPoint : points) {
            qreal currentDistance = qSqrt((currentPoint.x() - clickedPoint.x())
                                          * (currentPoint.x() - clickedPoint.x())
                                          + (currentPoint.y() - clickedPoint.y())
                                          * (currentPoint.y() - clickedPoint.y()));
            if (currentDistance < distance) {
                distance = currentDistance;
                closest = currentPoint;
            }
        }
    }
    findPolarizationAnalysisDataForClickedPoint(closest);
}

void PolarGraph::findPolarizationAnalysisDataForClickedPoint(const QPointF &point)
{

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
              _dataItem->setAnchor(QPointF(point.x() - 100, point.y() - 100));
              _dataItem->setText("hello");
              _dataItem->updateGeometry();
              break;
          }
    }
}
} // namespace EventOperation
















