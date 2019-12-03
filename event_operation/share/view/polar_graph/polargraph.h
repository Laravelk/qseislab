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

#include <data/seismwavepick.h>

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
} // namespace Data

QT_CHARTS_USE_NAMESPACE;

namespace EventOperation {
class PolarGraph : public QWidget {
  Q_OBJECT
public:
  PolarGraph(QWidget *parent = nullptr);
  QWidget *getView() const;
  void update(const Data::SeismEvent * const);
  void setGraphColor(const QBrush &);
  void setScatterColor(const QBrush&);
  void setAlarmAboutUnvalidData(bool);

private:
  QPolarChart *_polarChart;
  QChartView *_polarView;
  QValueAxis *_angularAxis;
  QValueAxis *_radialAxis;
  QWidget *_allView;
  QGraphicsTextItem *_status;
  QGraphicsRectItem *_statusRect;
  QList<QScatterSeries *> _seriesList;
  QList<Data::SeismPolarizationAnalysisData> _dataList;

  const QString WARNING_STATUS = "WARNING. UNVALID DATA";
};

} // namespace EventOperation
