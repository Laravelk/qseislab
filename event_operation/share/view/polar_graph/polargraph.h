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
#include <QtMath>
#include <memory>

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
} // namespace Data

QT_CHARTS_USE_NAMESPACE;

namespace EventOperation {
class PolarGraph : public QFrame {
  Q_OBJECT
public:
  PolarGraph(QWidget *parent = nullptr);
  QWidget *getView() const;

  void update(const Data::SeismEvent * const);

private:
  QPolarChart *_polarChart;
  QChartView *_polarView;
  QValueAxis *_angularAxis;
  QValueAxis *_radialAxis;
  QWidget *_allView;
  QList<QScatterSeries *> _seriesList;
};

} // namespace EventOperation
