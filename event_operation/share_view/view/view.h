#pragma once

#include "../chartgesture.h"
#include "wavepick.h"
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE

namespace EventOperation {
class View : public QChartView {
  Q_OBJECT
public:
  View(QChart *, QWidget *parent = nullptr);
  void addModel(ChartGesture *model) { _chart = model; }
  void addPick(WavePick *);
  void addPick(Data::SeismWavePick::Type, qreal, qreal, int, int, QBrush,
               qreal);
  void addPick(Data::SeismWavePick::Type, QPointF, QSize, QBrush, qreal);

  void setPWaveAddTriggerFlag(bool);
  void setSWaveAddTriggerFlag(bool);
  void setWaveRadius(qreal wr) { WAVE_RADIUS = wr; }
  void setRangeX(qreal rangeX) { _rangeX = rangeX; }

  QList<WavePick *> *getPickcs() { return &_wavePicks; }
  void clearPicks() {
    for (auto &pick : _wavePicks) {
      scene()->removeItem(pick);
    }
    _wavePicks.clear();
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
  bool mouseIsTouching = false;
  bool _isAddPWaveTriggerPressed = false;
  bool _isAddSWaveTriggerPressed = false;
  qreal _mFactor = 1.0;
  ChartGesture *_chart;
  QList<WavePick *> _wavePicks;

  QPointF calculatePickPosition(QPointF);
  bool checkAvailability(Data::SeismWavePick::Type, int);

signals:
  void sendTypeNumCompY(Data::SeismWavePick::Type, int, int);

private:
  QGraphicsRectItem *rect;
};
} // namespace EventOperation
