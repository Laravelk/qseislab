#include "graphic_view.h"

#include <QtGui/QMouseEvent>
#include <iostream>

#include <math.h>

//#include <iostream> // TODO: delete

namespace EventOperation {
GraphicView::GraphicView(QChart *chart, QWidget *parent)
    : QChartView(chart, parent), mouseIsTouching(false) {
  chart->setAnimationOptions(QChart::NoAnimation);
  setDragMode(QGraphicsView::NoDrag);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setRenderHint(QPainter::Antialiasing);
  rect = scene()->addRect(chart->plotArea());
  rect->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
  rect->setZValue(10);
  scene()->addItem(rect);

//  Pipes2DName *pipe = new Pipes2DName(chart);
//  pipe->setText("HELLOOOOOOOOOO");
//  pipe->setAnchor(QPointF(0, 0));
//  pipe->setZValue(11);
//  pipe->updateGeometry();
//  pipe->setRotation(270);
//  scene()->addItem(pipe);
//  pipe->show();
//  _pipesName.append(pipe);

//    text = new QGraphicsTextItem("HELLO HELLO HELLO",
//    chart); text->setTextWidth(300); text->setRotation(270);
//    text->setDefaultTextColor(Qt::black);
//    text->setPos(10, 218);
//    text->show();
}

void GraphicView::addPick(Data::SeismWavePick::Type type, qreal ax, qreal ay,
                          int width, int height, QBrush brush, qreal rangeX,
                          qreal leftBorderOffset, qreal rightBorderOffset) {
  addPick(type, QPointF(ax, ay), QSize(width, height), brush, rangeX,
          leftBorderOffset, rightBorderOffset);
}

void GraphicView::addPick(Data::SeismWavePick::Type type, QPointF pos,
                          QSizeF size, QBrush brush, qreal rangeX,
                          qreal leftBorderPos, qreal rightBorderPos) {
  QBrush borderBrush;
  std::cerr << chart()->mapToPosition(QPointF(100000,4)).y() << " ";
  if (Data::SeismWavePick::SWAVE == type) {
    borderBrush = Qt::darkGreen;
  } else {
    borderBrush = Qt::darkCyan;
  }
  WavePick *pick = new WavePick(type, rect, chart(), pos, size, brush, 2, 4);
  if (0 >= leftBorderPos) {
    leftBorderPos = 1;
  }

  //  if (rightBorderXOffset >= rangeX) {
  //    rightBorderXOffset = rangeX - 1;
  //  }
  WavePick *leftBorder =
      new WavePick(type, rect, chart(), QPointF(leftBorderPos, pos.y()), size,
                   borderBrush, 0, pick);
  WavePick *rightBorder =
      new WavePick(type, rect, chart(), QPointF(rightBorderPos, pos.y()), size,
                   borderBrush, pick, rangeX);
  connect(pick, &WavePick::changed, [this, pick, leftBorder, rightBorder]() {
    emit sendPicksInfo(pick->getType(), pick->getComponentNumber(),
                       leftBorder->getXPos(), pick->getXPos(),
                       rightBorder->getXPos());
  });
  connect(leftBorder, &WavePick::changed,
          [this, pick, leftBorder, rightBorder]() {
            emit sendPicksInfo(pick->getType(), pick->getComponentNumber(),
                               leftBorder->getXPos(), pick->getXPos(),
                               rightBorder->getXPos());
          });
  connect(rightBorder, &WavePick::changed,
          [this, pick, leftBorder, rightBorder]() {
            emit sendPicksInfo(pick->getType(), pick->getComponentNumber(),
                               leftBorder->getXPos(), pick->getXPos(),
                               rightBorder->getXPos());
          });

  pick->setBorders(leftBorder, rightBorder);
  pick->setZValue(11);
  pick->setScale(_currentlyWavesScale);
  leftBorder->setScale(_currentlyWavesScale);
  rightBorder->setScale(_currentlyWavesScale);
  leftBorder->setZValue(11);
  rightBorder->setZValue(11);
  _wavePicks.push_back(leftBorder);
  _wavePicks.push_back(rightBorder);
  _wavePicks.push_back(pick);
}

void GraphicView::setWaveAddTriggerFlag(Data::SeismWavePick::Type type) {
  if (type == Data::SeismWavePick::PWAVE) {
//      std::cerr << "SET PWAVE FLAG\n";
    _isAddPWaveTriggerPressed = true;
    _isAddSWaveTriggerPressed = false;
  } else if (type == Data::SeismWavePick::SWAVE) {
//      std::cerr << "SET SWAVE FLAG\n";
    _isAddSWaveTriggerPressed = true;
    _isAddPWaveTriggerPressed = false;
  }
  this->setFocus();
  _chart->setActive(true);

}

bool GraphicView::viewportEvent(QEvent *event) {
  if (event->type() == QEvent::TouchBegin) {
    mouseIsTouching = true;
  }
  return QChartView::viewportEvent(event);
}

void GraphicView::mousePressEvent(QMouseEvent *event) {
//    std::cerr << "clicked\n";
//    if (_isAddPWaveTriggerPressed) {
//        std::cerr << "add PWAVE PRESSED";
//    } else {
//        std::cerr << "add PWAVE NOT PRESSED\n";
//    }
//    if (_isAddSWaveTriggerPressed) {
//        std::cerr << "add SWAVE PRESSED";
//    } else {
//        std::cerr << "add SWAVE NOT PRESSED\n\n";
//    }
  if (_isAddPWaveTriggerPressed) {
//      std::cerr << "CREATE P WAVE\n";
    QPointF pos = calculatePickPosition(chart()->mapToValue(event->pos()));
    if (checkAvailability(Data::SeismWavePick::PWAVE,
                          static_cast<int>(pos.y()))) {
      addPick(Data::SeismWavePick::PWAVE, pos, QSize(2, 40), Qt::darkRed,
              _rangeX, pos.x() - 20000, pos.x() + 20000);
    }
    _isAddPWaveTriggerPressed = false;
  }

  if (_isAddSWaveTriggerPressed) {
//      std::cerr << "CREATE S WAVE\n";
    QPointF pos = calculatePickPosition(chart()->mapToValue(event->pos()));
    if (checkAvailability(Data::SeismWavePick::SWAVE,
                          static_cast<int>(pos.y()))) {
      addPick(Data::SeismWavePick::SWAVE, pos, QSize(2, 40), Qt::darkRed,
              _rangeX, pos.x() - 20000, pos.x() + 20000);
    }
    _isAddSWaveTriggerPressed = false;
  }

  QChartView::mousePressEvent(event);
  if (event->button() == Qt::RightButton) {
    for (auto &wave : _wavePicks) {
      wave->updateGeometry();
    }
  }
}

void GraphicView::mouseMoveEvent(QMouseEvent *event) {
  if (mouseIsTouching)
    return;
  QChartView::mouseMoveEvent(event);
}

void GraphicView::mouseReleaseEvent(QMouseEvent *event) {
//    std::cerr << "HHHH ";
  if (event->button() == Qt::RightButton) {
    if (scene()) {
      scaleContentsBy(0.7);
      return;
    }
  }
  if (mouseIsTouching) {
    mouseIsTouching = false;
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
  default:
    QGraphicsView::keyPressEvent(event);
    break;
  }
}

void GraphicView::keyReleaseEvent(QKeyEvent *event) {
  QChartView::keyReleaseEvent(event);
}

void GraphicView::mouseDoubleClickEvent(QMouseEvent *event) {
  QChartView::mouseDoubleClickEvent(event);
}

void GraphicView::paintEvent(QPaintEvent *event) {
  rect->setRect(chart()->plotArea());
  QChartView::paintEvent(event);
}

void GraphicView::scrollContentsBy(int dx, int dy) {
  if (scene()) {
    _chart->scroll(dx, dy);
    for (auto &wave : _wavePicks) {
      wave->updateGeometry();
    }
    text->setPos(text->pos().x(),chart()->mapToPosition(QPointF(100000,4)).y());

  }
}

void GraphicView::resizeEvent(QResizeEvent *event) {
  if (scene()) {
    scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    QSizeF scaleCoff;
    if (event->oldSize().width() != -1) {
      scaleCoff =
          QSizeF(1.0f * event->size().width() / event->oldSize().width(),
                 1.0f * event->size().height() / event->oldSize().height());
    } else {
      scaleCoff = QSizeF(1.0f, 1.0f);
    }
    _chart->resize(event->size());
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
  if (scene()) {
    _chart->zoom(factor);
    _currentlyWavesScale *= factor;
    for (auto &wavePick : _wavePicks) {
      wavePick->setScale(_currentlyWavesScale);
      wavePick->updateGeometry();
    }
    text->setScale(_currentlyWavesScale);
    text->setPos(text->pos().x(),chart()->mapToPosition(QPointF(100000,4)).y());
  }
}

QPointF GraphicView::calculatePickPosition(QPointF pointByMouse) {
  if (pointByMouse.y() > _countOfComponents - 1) {
    return QPointF(pointByMouse.x() - 500, _countOfComponents - 1);
  }

  if (pointByMouse.y() < 0) {
    return QPointF(pointByMouse.x() - 500, 0);
  }

  return QPointF(pointByMouse.x() - 500, round(pointByMouse.y()));
}

bool GraphicView::checkAvailability(Data::SeismWavePick::Type type, int index) {
  for (auto &wavePick : _wavePicks) {
    if (wavePick->getType() == type &&
        wavePick->getComponentNumber() == index) {
      return false;
    }
  }
  return true;
}
} // namespace EventOperation
