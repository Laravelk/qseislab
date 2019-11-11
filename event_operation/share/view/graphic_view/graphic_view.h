#pragma once

#include "../chartgesture.h"
#include "pipes2dname.h"
#include "wavepick.h"
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

#include <iostream> // TODO: delete

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE

namespace EventOperation {
class GraphicView : public QChartView {
  Q_OBJECT
public:
  GraphicView(QChart *, QWidget *parent = nullptr);

  void addModel(ChartGesture *model) { _chart = model; }
  void addPick(WavePick *);
  void addPick(Data::SeismWavePick::Type, qreal, qreal, int, int, QBrush, qreal,
               qreal, qreal);
  void addPick(Data::SeismWavePick::Type, QPointF, QSizeF, QBrush, qreal, qreal,
               qreal);

  void setWaveAddTriggerFlag(Data::SeismWavePick::Type);
  void setWaveRadius(const qreal wr) { WAVE_RADIUS = wr; }
  void setRangeX(const qreal rangeX) { _rangeX = rangeX; }
  void setCountOfComponents(int count) { _countOfComponents = count; }

  QList<WavePick *> *getPickcs() { return &_wavePicks; }
  const bool isEdit() const { return _editMode; }
  void clearPicks() {
    for (auto &pick : _wavePicks) {
      scene()->removeItem(pick);
    }
    _wavePicks.clear();
  }

  void mouseEvent(const QPointF &pos) {
      QPointF localPos = QPointF(_chart->mapToPosition(pos));
      this->mousePressEvent(new QMouseEvent(QEvent::MouseButtonPress, localPos, Qt::LeftButton, Qt::LeftButton,
                                                  Qt::NoModifier));
  }

protected:
  bool viewportEvent(QEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void keyPressEvent(QKeyEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;
  void mouseDoubleClickEvent(QMouseEvent *) override;
  void paintEvent(QPaintEvent *) override;
  void scrollContentsBy(int dx, int dy) override;
  void resizeEvent(QResizeEvent *) override;
  void wheelEvent(QWheelEvent *) override;
  void scaleContentsBy(qreal factor);

private:
  qreal WAVE_RADIUS;
  qreal _rangeX;
  int _countOfComponents = 0;
  bool _zoomIsTouching = false;
  bool _editMode = false;
  bool _addWaveMode = false;
  bool _isAddPWaveTriggerPressed = false;
  bool _isAddSWaveTriggerPressed = false;
  ChartGesture *_chart;
  QList<WavePick *> _wavePicks;
  QList<Pipes2DName *> _pipesName;
  QPointF calculatePickPosition(QPointF);
  bool checkAvailability(Data::SeismWavePick::Type, int);
  QGraphicsTextItem *_status;
  QRubberBand *rubberBand = nullptr;

  const int MICROSECONDS_IN_SECOND = 1000000;
  const int MICROSECONDS_IN_MILISECOND = 1000;
  QPoint _firstPoint;

signals:
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void removePick(Data::SeismWavePick::Type, int);

private:
  QGraphicsRectItem *rect;

  const QString EDIT_MODE_STRING = "Edit Mode";
  const QString OVERVIEW_MODE_STRING = "Overview Mode";
  const QString ADD_WAVE_STRING = "Add Wave";
};
} // namespace EventOperation
