#include "polargraph.h"

#include <data/seismevent.h>

#include <iostream> // TODO: delete
#include <limits>

#include "event_operation/share/view/graphic_view/graphic_view.h"

// тип волны и номер компоненты
// по второму нажатию удаляем

namespace EventOperation {
PolarGraph::PolarGraph(QPolarChart *chart, QWidget *)
    : QChartView(chart), _polarChart(chart), _angularAxis(new QValueAxis()),
      _radialAxis(new QValueAxis), _rect(new QGraphicsRectItem) {

  setFrameStyle(1);
  const qreal angularMin = 0;
  const qreal angularMax = 360;

  const qreal radialMin = 0;
  const qreal radialMax = 180;

  this->setMinimumSize(600, 400);

  scene()->addRect(_polarChart->plotArea());

  _angularAxis->setTickCount(9);
  _angularAxis->setLabelFormat("%.1f");
  _angularAxis->setShadesBrush(QBrush(QColor(249, 249, 250)));
//  _angularAxis->setReverse(true);
//  _radialAxis->setReverse(true);
  _polarChart->addAxis(_angularAxis, QPolarChart::PolarOrientationAngular);
  _polarChart->legend()->hide();

  _radialAxis->setTickCount(9);
  _radialAxis->setLabelFormat("%.1f");
  _polarChart->addAxis(_radialAxis, QPolarChart::PolarOrientationRadial);

  _radialAxis->setRange(radialMin, radialMax);
  _angularAxis->setRange(angularMin, angularMax);

  this->setRenderHint(QPainter::Antialiasing);
  _statusRect = new QGraphicsRectItem(WARNING_STATUS_RECT, _polarChart);
  _statusRect->setZValue(11);
  _statusRect->setBrush(Qt::yellow);
  _status = new QGraphicsTextItem(WARNING_STATUS, _polarChart);
  _status->setPos(QPointF(20, 415));
  _status->setZValue(12);

  _status->hide();
  _statusRect->hide();
}

QWidget *PolarGraph::getView() const { return _allView; }

void PolarGraph::update(Data::SeismEvent const *const event) {
  _polarChart->removeAllSeries();
  _infoAboutPoint.clear();
  _seriesList.clear();
  _dataList.clear();
  QScatterSeries *validPWaveSeries = new QScatterSeries();
  validPWaveSeries->setColor(Qt::darkRed);
  QScatterSeries *validSWaveSeries = new QScatterSeries();
  validSWaveSeries->setColor(Qt::darkBlue);
  QScatterSeries *unValidSeries = new QScatterSeries();
  unValidSeries->setColor(Qt::gray);
  int num = 0;
  for (auto &component : event->getComponents()) {
    num++;
    for (auto &pick : component->getWavePicks()) {
      if (pick.second.getPolarizationAnalysisData() != std::nullopt) {
        Data::SeismPolarizationAnalysisData data =
            pick.second.getPolarizationAnalysisData().value();
        double polarAngle = 180 * sin(data.getIncidenceInRadian())/ (1 - cos(data.getIncidenceInRadian())) / M_PI;
        double azimut = data.getAzimutDegrees();

        if (polarAngle < 0) {
            polarAngle += PI_IN_DEGREES;
        }

        if (azimut < 0) {
            azimut += TWO_PI_IN_DEGREES;
        }
        if (data.isValid()) {
          if (pick.first == Data::SeismWavePick::PWAVE) {
            validPWaveSeries->append(azimut, polarAngle);
            _infoAboutPoint.push_back(PointInfo(num, Data::SeismWavePick::PWAVE, azimut, polarAngle,
                                                data.getRectilinearity(), data.getPlanarity()));
          } else {
            _infoAboutPoint.push_back(PointInfo(num, Data::SeismWavePick::SWAVE, azimut, polarAngle,
                                                data.getRectilinearity(), data.getPlanarity()));
            validSWaveSeries->append(azimut, polarAngle);
          }
        } else {
          unValidSeries->append(azimut, polarAngle);
        }
        _dataList.append(pick.second.getPolarizationAnalysisData().value());
      }
    }
  }
  if (!_hidePWave) {
    _seriesList.append(validPWaveSeries);
    _polarChart->addSeries(validPWaveSeries);
    validPWaveSeries->attachAxis(_radialAxis);
    validPWaveSeries->attachAxis(_angularAxis);
  }
  if (!_hideSWave) {
    _seriesList.append(validSWaveSeries);
    _polarChart->addSeries(validSWaveSeries);
    validSWaveSeries->attachAxis(_radialAxis);
    validSWaveSeries->attachAxis(_angularAxis);
  }

  // connet zone
  connect(validPWaveSeries, &QScatterSeries::clicked,
          [this](const QPointF &point) { handleClickedPoint(point); });

  connect(validSWaveSeries, &QScatterSeries::clicked,
          [this](const QPointF &point) { handleClickedPoint(point); });

  connect(unValidSeries, &QScatterSeries::clicked,
          [this](const QPointF &point) { handleClickedPoint(point); });

  // end connect zone

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
}

void PolarGraph::loadEvent(const Data::SeismEvent * const event)
{
    update(event);
}

void PolarGraph::setGraphColor(const QBrush &) {
}

void PolarGraph::setScatterColor(const QBrush &color) {
  for (auto &series : _seriesList) {
    series->setColor(color.color());
  }
}

void PolarGraph::hideSWavePoints(bool hide) { _hideSWave = hide; }

void PolarGraph::hidePWavePoints(bool hide) { _hidePWave = hide; }

void PolarGraph::setAngularMin(int value)
{
    _angularAxis->setMin(value);
    hideAllInfo();
}

void PolarGraph::setAngularMax(int value)
{
    _angularAxis->setMax(value);
    hideAllInfo();
}

void PolarGraph::setRadialMin(int value)
{
    _radialAxis->setMin(value);
    hideAllInfo();
}

void PolarGraph::setRadialMax(int value)
{
    _radialAxis->setMax(value);
    hideAllInfo();
}

void PolarGraph::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Alt: {
    break;
  }
  case Qt::Key_1: {
       hideAllInfo();
       break;
  }
  case Qt::Key_2: {
        _angularAxis->setMin(0);
        _angularAxis->setMax(1000);
        _radialAxis->setMin(90);
        _radialAxis->setMax(300);
      break;
  }

  default:
    QGraphicsView::keyPressEvent(event);
    break;
  }
}

void PolarGraph::mousePressEvent(QMouseEvent *event) {
  QChartView::mousePressEvent(event);
}

void PolarGraph::mouseMoveEvent(QMouseEvent *event)
{
 QChartView::mouseMoveEvent(event);
}

void PolarGraph::mouseReleaseEvent(QMouseEvent *event)
{
    QChartView::mouseReleaseEvent(event);
}

void PolarGraph::keyReleaseEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Alt:
    break;
 case Qt::Key_Plus:
      _polarChart->zoomIn();
      break;
 case Qt::Key_Minus:
      _polarChart->zoomOut();
      break;
  default:
    QChartView::keyReleaseEvent(event);
    break;
  }
}

void PolarGraph::handleClickedPoint(const QPointF &point) {
  QPointF clickedPoint = point;
  QPointF closest(INT_MAX, INT_MAX);
  qreal distance(INT_MAX);
  for (auto &series : _seriesList) {
    auto points = series->points();
    for (const QPointF &currentPoint : points) {
      qreal currentDistance = qSqrt((currentPoint.x() - clickedPoint.x()) *
                                        (currentPoint.x() - clickedPoint.x()) +
                                    (currentPoint.y() - clickedPoint.y()) *
                                        (currentPoint.y() - clickedPoint.y()));
      if (currentDistance < distance) {
        distance = currentDistance;
        closest = currentPoint;
      }
    }
  }
  findPolarizationAnalysisDataForClickedPoint(closest);
}

void PolarGraph::findPolarizationAnalysisDataForClickedPoint(
    const QPointF &point) {
  for (auto &data : _dataList) {
    qreal dataPolarAngle = 180 * sin(data.getIncidenceInRadian())/ (1 - cos(data.getIncidenceInRadian())) / M_PI;
    qreal dataAzimutAngle =
        static_cast<qreal>(data.getAzimutDegrees());
    if (dataAzimutAngle < 0) {
        dataAzimutAngle += TWO_PI_IN_DEGREES;
    }
    if (dataPolarAngle < 0) {
        dataAzimutAngle += PI_IN_DEGREES;
    }
    if (compareFloat(dataPolarAngle, point.y()) && compareFloat(dataAzimutAngle, point.x())) {
      Data::SeismWavePick::Type ownerEventType = Data::SeismWavePick::SWAVE;
      int ownerNumber = 0;
      float planarity = 0;
      float rectilinear = 0;
      PointInfo *currentPoint;
      for (auto &pointData : _infoAboutPoint) {
        if (compareFloat(dataPolarAngle, pointData.polarAngle) && compareFloat(dataAzimutAngle, pointData.azimutAngle)) {
            ownerEventType = pointData.type;
            ownerNumber = pointData.numberOfComponents;
            planarity = pointData.planarity;
            rectilinear = pointData.rectilinear;
            pointData.isShowing = !pointData.isShowing;
            currentPoint = &pointData;
            if (false == pointData.isShowing) {
                pointData.windowWithInfo->hide();
                return;
            }
        }
      }

      if (currentPoint->windowWithInfo == nullptr) {
          currentPoint->windowWithInfo = new AnalysisDataGraphicItem(_polarChart);
      }

    currentPoint->windowWithInfo->setAnchor(point);
    if (ownerEventType == Data::SeismWavePick::PWAVE) {
        currentPoint->windowWithInfo->setText(
            QString("Polar angle: %1 \nAzimut: %2 \nWaveType: PWAVE \nReceiver number: %3 \nPlanarity: %4 \nRectilinearity %5")
                    .arg(dataPolarAngle).arg(dataAzimutAngle).
                    arg(ownerNumber).arg(planarity).arg(rectilinear));
    } else {
        currentPoint->windowWithInfo->setText(
            QString("Polar angle: %1 \nAzimut: %2 \nWaveType: SWAVE \nReceiver number: %3 \nPlanarity: %4 \nRectilinearity %5").
                    arg(dataPolarAngle).arg(dataAzimutAngle).
                    arg(ownerNumber).arg(planarity).arg(rectilinear));
    }

    currentPoint->windowWithInfo->setZValue(11);
    currentPoint->windowWithInfo->updateGeometry();
    currentPoint->windowWithInfo->show();
    _hideAllInfo = false;
    break;
    }
  }
}

bool PolarGraph::compareFloat(float a, float b)
{
    return (std::fabs(a - b) <
            std::numeric_limits<qreal>::epsilon());
}

void PolarGraph::hideAllInfo()
{
    if (_hideAllInfo) {
        return;
    }

    for (auto &info : _infoAboutPoint) {
      info.isShowing = false;
      if (info.windowWithInfo != nullptr) {
         info.windowWithInfo->hide();
      }
    }
    _hideAllInfo = true;
    return;
}

} // namespace EventOperation
