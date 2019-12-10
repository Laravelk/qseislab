#pragma once

#include <QHBoxLayout>
#include <QList>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPolarChart>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QLabel>
#include <QtMath>
#include <memory>

#include "data/seismwavepick.h"
#include "analysisdatagraphicsitem.h"

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
} // namespace Data

QT_CHARTS_USE_NAMESPACE;

namespace EventOperation {
class PolarGraph : public QChartView {
  Q_OBJECT
public:
  PolarGraph(QPolarChart *chart, QWidget *parent = nullptr);
  QWidget *getView() const;
  void update(const Data::SeismEvent * const);
  void setGraphColor(const QBrush &);
  void setScatterColor(const QBrush&);

  void hideSWavePoints(bool);
  void hidePWavePoints(bool);

protected:
  void keyPressEvent(QKeyEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;

private:
  QPolarChart *_polarChart;
  QGraphicsRectItem *_rect;
  QValueAxis *_angularAxis;
  QValueAxis *_radialAxis;
  QWidget *_allView;
  QGraphicsTextItem *_status;
  QGraphicsTextItem *_info = nullptr;
  QGraphicsRectItem *_statusRect;
  QGraphicsRectItem *_infoRect = nullptr;
  QList<QScatterSeries *> _seriesList;
  QList<Data::SeismPolarizationAnalysisData> _dataList;
  AnalysisDataGraphicsItem *_dataItem = nullptr;

  bool _hideSWave = false;
  bool _hidePWave = false;
  bool _altIsTouch = false;

  const QString WARNING_STATUS = "WARNING. DATA WAS CHANGE";
  const QString ALT_IS_TOUCHING_STATUS = "EDIT MODE";
  const QRectF WARNING_STATUS_RECT = QRectF(19, 413, 158, 20);
  const QString NORMAL_STATUS = "OK";

  void handleClickedPoint(const QPointF &);
  void findPolarizationAnalysisDataForClickedPoint(const QPointF &);
};

} // namespace EventOperation
