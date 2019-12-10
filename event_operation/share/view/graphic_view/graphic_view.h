#pragma once

#include "../chartgesture.h"
#include "wavepick.h"
#include "viewoperation.h"
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

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
  void addPick(Data::SeismWavePick::Type, qreal, qreal, qreal,
               qreal, qreal);
  void addPick(Data::SeismWavePick::Type, QPointF, qreal, qreal,
               qreal);

  const QColor getAxisColor(const unsigned int componentIndexInRecieverData) const {
      return _colorData->getComponentColor(componentIndexInRecieverData);
  }

  void setWaveAddTriggerFlag(Data::SeismWavePick::Type);
  void setWaveRadius(const qreal wr) { WAVE_RADIUS = wr; }
  void setRangeX(const qreal rangeX) { _rangeX = rangeX; }
  void setCountOfComponents(int count) { _countOfComponents = count; }

  QList<WavePick *> *getPickcs() { return &_wavePicks; }
  bool isEdit() const { return _editMode; }
  void clearView() {
    for (auto &pick : _wavePicks) {
      scene()->removeItem(pick);
    }
    _wavePicks.clear();
//    std::cerr << "wave picks after clear size " << _wavePicks.size() << std::endl;
  }

  void clearHistoryOfTransformations();
  void useHistoryOfTransformations();

  void mouseEvent(const QPointF &pos) {
      QPointF localPos = QPointF(_chart->mapToPosition(pos));
      this->mousePressEvent(new QMouseEvent(QEvent::MouseButtonPress, localPos, Qt::LeftButton, Qt::LeftButton,
                                                  Qt::NoModifier));
  }

  void resetItemSize() { _sizeWaveItem = DEFAULT_WAVEITEM_SIZE;}

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
  QPointF calculatePickPosition(QPointF);
  bool checkAvailability(Data::SeismWavePick::Type, int);
  QGraphicsTextItem *_status;
  QRubberBand *rubberBand = nullptr;
  QPoint _firstPoint;
  QSizeF _sizeWaveItem = QSizeF(2,40);

signals:
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void removePick(Data::SeismWavePick::Type, int);
  void addPickSignal(Data::SeismWavePick::Type, int, int, int, int);

private:
  QGraphicsRectItem *rect;

  const QString EDIT_MODE_STRING = "Edit Mode";
  const QString OVERVIEW_MODE_STRING = "Overview Mode";
  const QString ADD_WAVE_STRING = "Add Wave";
  const int MICROSECONDS_IN_SECOND = 1000000;
  const int MICROSECONDS_IN_MILISECOND = 1000;
  const QSizeF DEFAULT_WAVEITEM_SIZE = QSizeF(2,40);

private:
  QList<ViewOperation> _transformationsZoomHistory;

  class ColorData {
  public:
      explicit ColorData() {
            fillPickColor();
            fillBorderPickColor();
      }
      const QColor getPickColor(const Data::SeismWavePick::Type type) {
          return _pickColor.at(type);
      }
      const QColor getBorderPickColor(const Data::SeismWavePick::Type type) const {
          return _borderColor.at(type);
      }
      const QColor getComponentColor(const unsigned int componentIndexInRecieverData) const {
          return _componentColors[componentIndexInRecieverData];
      }

  private:
      std::map<Data::SeismWavePick::Type, QColor> _pickColor;
      std::map<Data::SeismWavePick::Type,  QColor> _borderColor;
      const QColor _componentColors[3] = {QColor(220, 20, 60), QColor(50, 205, 50),
                              QColor(65, 105, 225)};

      void fillPickColor() {
          _pickColor[Data::SeismWavePick::PWAVE] = Qt::darkRed;
          _pickColor[Data::SeismWavePick::SWAVE] = Qt::darkBlue;
      }

      void fillBorderPickColor() {
          _borderColor[Data::SeismWavePick::PWAVE] = Qt::darkCyan;
          _borderColor[Data::SeismWavePick::SWAVE] = Qt::darkGreen;
      }
  };

  ColorData *_colorData;
};
} // namespace EventOperation
