#include "graphic_view.h"

#include <QtGui/QMouseEvent>
#include <iostream>

#include <QDebug>
#include <math.h>

namespace EventOperation {
GraphicView::GraphicView(QChart *chart, QWidget *parent)
    : QChartView(chart, parent), _zoomIsTouching(false),
      _colorData(new ColorData) {
  chart->setAnimationOptions(QChart::NoAnimation);
  chart->setMargins(QMargins(0, 0, 0, 0));
  setDragMode(QGraphicsView::NoDrag);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setRenderHint(QPainter::Antialiasing);
  rect = scene()->addRect(chart->plotArea());
  rect->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
  rect->setZValue(10);
  _status = new QGraphicsTextItem(OVERVIEW_MODE_STRING, this->chart());
  _status->setPos(QPointF(20, 450));
  _status->show();
  this->chart()->zoomReset();
}

void GraphicView::addPick(Data::SeismWavePick::Type type, qreal ax, qreal ay,
                          qreal rangeX, qreal leftBorderOffset,
                          qreal rightBorderOffset) {
  addPick(type, QPointF(ax, ay), rangeX, leftBorderOffset, rightBorderOffset);
}

void GraphicView::addPick(Data::SeismWavePick::Type type, QPointF pos,
                          qreal rangeX, qreal leftBorderPos,
                          qreal rightBorderPos) {
  WavePick *pick = new WavePick(type, rect, chart(), pos, _sizeWaveItem,
                                _colorData->getPickColor(type), 2, 4);
  WavePick *leftBorder = new WavePick(
      type, rect, chart(), QPointF(leftBorderPos, pos.y()), _sizeWaveItem,
      _colorData->getBorderPickColor(type), 0, pick);
  WavePick *rightBorder = new WavePick(
      type, rect, chart(), QPointF(rightBorderPos, pos.y()), _sizeWaveItem,
      _colorData->getBorderPickColor(type), pick, rangeX);
  pick->setBorders(leftBorder, rightBorder);
  connect(pick, &WavePick::changed, [this, pick, leftBorder, rightBorder]() {
      leftBorder->setRightBorderValue(pick->getXPos());
      rightBorder->setLeftBorderValue(pick->getXPos());
    emit sendPicksInfo(pick->getType(), pick->getComponentAmount(),
                       static_cast<int>(leftBorder->getXPos() * MICROSECONDS_IN_SECOND),
                       static_cast<int>(pick->getXPos() * MICROSECONDS_IN_SECOND),
                       static_cast<int>(rightBorder->getXPos() * MICROSECONDS_IN_SECOND));
  });
  connect(leftBorder, &WavePick::changed,
          [this, pick, leftBorder, rightBorder]() {
            pick->setLeftBorderValue(leftBorder->getXPos());
            emit sendPicksInfo(pick->getType(), pick->getComponentAmount(),
                               static_cast<int>(leftBorder->getXPos() * MICROSECONDS_IN_SECOND),
                               static_cast<int>(pick->getXPos() * MICROSECONDS_IN_SECOND),
                               static_cast<int>(rightBorder->getXPos() * MICROSECONDS_IN_SECOND));
          });
  connect(rightBorder, &WavePick::changed,
          [this, pick, leftBorder, rightBorder]() {
            pick->setRightBorderValue(rightBorder->getXPos());
            emit sendPicksInfo(pick->getType(), pick->getComponentAmount(),
                               static_cast<int>(leftBorder->getXPos() * MICROSECONDS_IN_SECOND),
                               static_cast<int>(pick->getXPos() * MICROSECONDS_IN_SECOND),
                               static_cast<int>(rightBorder->getXPos() * MICROSECONDS_IN_SECOND));
          });

  connect(pick, &WavePick::needDelete, [this, pick, leftBorder, rightBorder]() {
//      std::cerr << "del pick" << std::endl;
      emit removePick(pick->getType(), pick->getComponentAmount());
  });
  _wavePicks.push_back(leftBorder);
  _wavePicks.push_back(rightBorder);
  _wavePicks.push_back(pick);
  for (auto &pick : _wavePicks) {
    pick->updateGeometry();
  }
}

void GraphicView::setWaveAddTriggerFlag(Data::SeismWavePick::Type type) {
  if (!_editMode) {
    _addWaveMode = true;
    _status->setPlainText(ADD_WAVE_STRING);
  }
  if (type == Data::SeismWavePick::PWAVE) {
    _isAddPWaveTriggerPressed = true;
    _isAddSWaveTriggerPressed = false;
  } else if (type == Data::SeismWavePick::SWAVE) {
    _isAddSWaveTriggerPressed = true;
    _isAddPWaveTriggerPressed = false;
  }
  this->setFocus();
  _chart->setActive(true);
}

void GraphicView::clearHistoryOfTransformations() {
  _sizeWaveItem = DEFAULT_WAVEITEM_SIZE;
  _transformationsZoomHistory.clear();
}

void GraphicView::useHistoryOfTransformations() {
  chart()->zoomReset();
  _sizeWaveItem = DEFAULT_WAVEITEM_SIZE;
  std::cerr << " count of operation " << _transformationsZoomHistory.size()
            << std::endl;
  for (auto &operation : _transformationsZoomHistory) {
      if (ViewOperation::OPERATION_TYPE::ZOOM == operation.operationType() ) {
          std::cerr << "h zoom " << operation.factor() << std::endl;
          chart()->zoom(operation.factor());
          //            _sizeWaveItem = QSizeF(_sizeWaveItem.width(), _sizeWaveItem.height() * operation.factor());
          //            for (auto &wavePick : _wavePicks) {
          //            _sizeWaveItem = wavePick->scallByAxis(QSizeF(operation.factor(), operation.factor()));
          //            wavePick->updateGeometry();
          //            }
      }
      if (ViewOperation::OPERATION_TYPE::SCROLL == operation.operationType()) {
          chart()->scroll(operation.dx(), operation.dy());
      }
      if (ViewOperation::OPERATION_TYPE::ZOOM_IN == operation.operationType()) {
          //            _sizeWaveItem = QSizeF(_sizeWaveItem.width(), _sizeWaveItem.height() * operation.factor());
          //            for (auto &pick : _wavePicks) {
          //                _sizeWaveItem = pick->scallByAxis(QSizeF(1.0f, operation.factor()));
          //                pick->updateGeometry();
          //            }
          chart()->zoomIn(operation.rect());
      }
  }
  for (auto &wave : _wavePicks) {
      wave->updateGeometry();
  }

}

bool GraphicView::viewportEvent(QEvent *event) {
  if (event->type() == QEvent::TouchBegin) {
    _zoomIsTouching = true;
    chart()->setAnimationOptions(QChart::NoAnimation);
  }
  return QChartView::viewportEvent(event);
}

void GraphicView::mousePressEvent(QMouseEvent *event) {
  if (!_editMode && (event->buttons() == Qt::LeftButton)) {
    QPoint origin = event->pos();
    if (!rubberBand) {
      rubberBand = new QRubberBand(QRubberBand::Line, this);
    }
    _firstPoint = origin;
    rubberBand->setGeometry(QRect(origin, QSize()));
    _zoomIsTouching = true;
    rubberBand->show();
  }

  if ((_isAddPWaveTriggerPressed && _editMode) ||
      (_isAddPWaveTriggerPressed && _addWaveMode)) {
    QPointF pos = calculatePickPosition(chart()->mapToValue(event->pos()));
    if (checkAvailability(Data::SeismWavePick::PWAVE,
                          static_cast<int>(pos.y()))) {
      emit addPickSignal(Data::SeismWavePick::PWAVE, pos.y() ,(pos.x() - 0.025) * MICROSECONDS_IN_SECOND, pos.x() * MICROSECONDS_IN_SECOND,
                           (pos.x() + 0.025) * MICROSECONDS_IN_SECOND);
      addPick(Data::SeismWavePick::PWAVE, pos, _rangeX, pos.x() - 0.025,
              pos.x() + 0.025);
    }
    _isAddPWaveTriggerPressed = false;
  }

  if ((_isAddSWaveTriggerPressed && _editMode) ||
      (_isAddSWaveTriggerPressed && _addWaveMode)) {
    QPointF pos = calculatePickPosition(chart()->mapToValue(event->pos()));
    if (checkAvailability(Data::SeismWavePick::SWAVE,
                          static_cast<int>(pos.y()))) {
        emit addPickSignal(Data::SeismWavePick::SWAVE, pos.y() ,(pos.x() - 0.025) * MICROSECONDS_IN_SECOND, pos.x() * MICROSECONDS_IN_SECOND,
                           (pos.x() + 0.025) * MICROSECONDS_IN_SECOND);
      addPick(Data::SeismWavePick::SWAVE, pos, _rangeX, pos.x() - 0.025,
              pos.x() + 0.025);
    }
    _isAddSWaveTriggerPressed = false;
  }

  if (_addWaveMode) {
    _status->setPlainText(OVERVIEW_MODE_STRING);
    _addWaveMode = false;
  }

  if (event->button() == Qt::RightButton && _editMode) {
    for (auto &wave : _wavePicks) {
      wave->updateGeometry();
    }
  }
  QChartView::mousePressEvent(event);
}

void GraphicView::mouseMoveEvent(QMouseEvent *event) {
  if (!_editMode && _zoomIsTouching && event->buttons() == Qt::LeftButton) {
    if (rubberBand) {
      rubberBand->setGeometry(QRect(_firstPoint, event->pos()).normalized());
      rubberBand->show();
    }
  }
  QChartView::mouseMoveEvent(event);
}

void GraphicView::mouseReleaseEvent(QMouseEvent *event) {
    if (rubberBand && (rubberBand->width() > 10 || rubberBand->height() > 10)) {
        rubberBand->hide();
    }

    if (rubberBand && rubberBand->width() > 10 && rubberBand->height() > 10 && _zoomIsTouching && event->button() == Qt::LeftButton) {
        rubberBand->hide();
        QPoint point = rubberBand->pos();
        QSize size = rubberBand->size();
        delete rubberBand;
        QRect transform(point, size);
        ViewOperation operation(ViewOperation::OPERATION_TYPE::ZOOM_IN);
        operation.setRect(transform);
        _chart->zoomIn(transform);
        rubberBand = nullptr;
        float scale = std::min(chart()->plotArea().width() / size.width(), chart()->plotArea().height() / size.height());
        if (abs(scale) > 400) {
            return;
        }
        operation.setFactor(scale);
        _transformationsZoomHistory.push_back(operation);
        _sizeWaveItem = QSizeF(_sizeWaveItem.width(), _sizeWaveItem.height() * scale);
        for (auto &pick : _wavePicks) {
            _sizeWaveItem = pick->scallByAxis(QSizeF(chart()->plotArea().width() / size.width(), chart()->plotArea().height() / size.height()));
            pick->updateGeometry();
        }
        _zoomIsTouching = false;
        return;
    }


    if (event->button() == Qt::RightButton) {
        if (scene()) {
            scaleContentsBy(0.7);
            return;
        }
    }
    if (_zoomIsTouching) {
        _zoomIsTouching = false;
    }
    QChartView::mouseReleaseEvent(event);
}

void GraphicView::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Plus:
    scaleContentsBy(2);
    break;
  case Qt::Key_Minus:
    scaleContentsBy(0.5);
    break;
  case Qt::Key_Left:
    scrollContentsBy(-10, 0);
    break;
  case Qt::Key_A:
    scrollContentsBy(-10, 0);
    break;
  case Qt::Key_Right:
    scrollContentsBy(10, 0);
    break;
  case Qt::Key_D:
    scrollContentsBy(10, 0);
    break;
  case Qt::Key_Up:
    scrollContentsBy(0, 10);
    break;
  case Qt::Key_W:
    scrollContentsBy(0, 10);
    break;
  case Qt::Key_Down:
    scrollContentsBy(0, -10);
    break;
  case Qt::Key_S:
    scrollContentsBy(0, -10);
    break;
  case Qt::Key_Alt: {
//      std::cerr << _wavePicks.size() << std::endl;
      for (auto &pick : _wavePicks) {
        pick->setEditable(true);
      }
    _editMode = true;
    _status->setPlainText(EDIT_MODE_STRING);
    break;
  }
  default:
    QGraphicsView::keyPressEvent(event);
    break;
  }
}

void GraphicView::keyReleaseEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Alt:
    _editMode = false;
    _status->setPlainText(OVERVIEW_MODE_STRING);
    for (auto &pick : _wavePicks) {
      pick->setEditable(false);
    }
    break;
  default:
    QChartView::keyReleaseEvent(event);
    break;
  }
}

void GraphicView::mouseDoubleClickEvent(QMouseEvent *event) {
  QChartView::mouseDoubleClickEvent(event);
}

void GraphicView::paintEvent(QPaintEvent *event) {
  rect->setRect(chart()->plotArea());
  QChartView::paintEvent(event);
}

void GraphicView::scrollContentsBy(int dx, int dy) {
  if (!_editMode) {
    if (scene()) {
      _chart->scroll(dx, dy);
      ViewOperation operation(ViewOperation::OPERATION_TYPE::SCROLL);
      operation.setScroll(dx, dy);
      _transformationsZoomHistory.push_back(operation);
      for (auto &wave : _wavePicks) {
        wave->updateGeometry();
      }
    }
  }
}
//}

void GraphicView::resizeEvent(QResizeEvent *event) {
  if (scene()) {
    scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    QSizeF scaleCoff;
    if (event->oldSize().width() != -1) {
      scaleCoff = QSizeF(static_cast<double>(1.0f * event->size().width() /
                                             event->oldSize().width()),
                         static_cast<double>(1.0f * event->size().height() /
                                             event->oldSize().height()));
    } else {
      scaleCoff = QSizeF(static_cast<double>(1.0f), static_cast<double>(1.0f));
    }
    _chart->resize(event->size());
    _status->setPos(_status->pos().x() * scaleCoff.width(),
                    _status->pos().y() * scaleCoff.height());
    for (auto &wave : _wavePicks) {
      wave->resize(scaleCoff);
      wave->updateGeometry();
    }
  }
  QGraphicsView::resizeEvent(event);
}

// TODO: uncomment for wheelEvent on Windows
void GraphicView::wheelEvent(QWheelEvent *event) {
  qreal factor = event->angleDelta().y() > 0 ? 0.7 : 1.3;
  scaleContentsBy(factor);
  QChartView::wheelEvent(event);
}

void GraphicView::scaleContentsBy(qreal factor) {
  if (!_editMode) {
    _chart->zoom(factor);
    ViewOperation operation(ViewOperation::OPERATION_TYPE::ZOOM);
    operation.setFactor(factor);
    _transformationsZoomHistory.push_back(operation);
    if (scene()) {
      _sizeWaveItem =
          QSizeF(_sizeWaveItem.width(), _sizeWaveItem.height() * factor);
      for (auto &wavePick : _wavePicks) {
        _sizeWaveItem = wavePick->scallByAxis(QSizeF(factor, factor));
        wavePick->updateGeometry();
      }
    }
  }
}

QPointF GraphicView::calculatePickPosition(QPointF pointByMouse) {
  if (pointByMouse.y() > _countOfComponents - 1) {
    return QPointF(pointByMouse.x(), _countOfComponents - 1);
  }

  if (pointByMouse.y() < 0) {
    return QPointF(pointByMouse.x(), 0);
  }

  return QPointF(pointByMouse.x(), round(pointByMouse.y()));
}

bool GraphicView::checkAvailability(Data::SeismWavePick::Type type, int index) {
  for (auto &wavePick : _wavePicks) {
    if (wavePick->getType() == type &&
        wavePick->getComponentAmount() == index) {
      return false;
    }
  }
  return true;
}
} // namespace EventOperation
