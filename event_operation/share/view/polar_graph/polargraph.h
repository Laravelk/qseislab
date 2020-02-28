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

#include "analysisdatagraphicsitem.h"
#include "data/seismwavepick.h"

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
  void update(const Data::SeismEvent *const);
  void loadEvent(const Data::SeismEvent *const);
  void setGraphColor(const QBrush &);
  void setScatterColor(const QBrush &);

  void hideSWavePoints(bool);
  void hidePWavePoints(bool);

  void setAngularMin(int);
  void setAngularMax(int);
  void setRadialMin(int);
  void setRadialMax(int);

protected:
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;

private:
  struct PointInfo {
    PointInfo(int num, Data::SeismWavePick::Type pointType, double _azimutAngle,
                 double _polarAngle, float _rectilinear, float _planarity) :
        numberOfComponents(num), type(pointType), polarAngle(_polarAngle),
                 azimutAngle(_azimutAngle), planarity(_planarity), rectilinear(_rectilinear) {}
    ~PointInfo() { if (windowWithInfo != nullptr) {
                        delete windowWithInfo;
                        }
                 }
    int numberOfComponents;
    Data::SeismWavePick::Type type;
    double polarAngle;
    double azimutAngle;
    float planarity;
    float rectilinear;
    bool isShowing = false;
    AnalysisDataGraphicItem *windowWithInfo = nullptr;
  };
  QPolarChart *_polarChart;
  QGraphicsRectItem *_rect;
  QValueAxis *_angularAxis;
  QValueAxis *_radialAxis;
  QWidget *_allView;
  QGraphicsTextItem *_status;
  QGraphicsRectItem *_statusRect;
  QList<QScatterSeries *> _seriesList;
  std::vector<PointInfo> _infoAboutPoint;
  QList<Data::SeismPolarizationAnalysisData> _dataList;
  AnalysisDataGraphicItem *_dataItem = nullptr;
  bool _hideSWave = false;
  bool _hidePWave = false;
  bool _isZoomed = false;
  bool _hideAllInfo = true;

  const QString WARNING_STATUS = "WARNING. DATA WAS CHANGE";
  const QString ALT_IS_TOUCHING_STATUS = "EDIT MODE";
  const QRectF WARNING_STATUS_RECT = QRectF(19, 413, 158, 20);
  const QString NORMAL_STATUS = "OK";

  void handleClickedPoint(const QPointF &);
  void findPolarizationAnalysisDataForClickedPoint(const QPointF &);
  bool compareFloat(float a, float b);
  void hideAllInfo();

};

} // namespace EventOperation
