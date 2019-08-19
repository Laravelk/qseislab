#pragma once

#include <QtDataVisualization/Q3DInputHandler>
#include <QtDataVisualization/QAbstract3DGraph>
#include <QtDataVisualization/QValue3DAxis>

using namespace QtDataVisualization;

class CustomInputHandler : public Q3DInputHandler {
public:
  CustomInputHandler(QAbstract3DGraph *graph, QObject *parent)
      : Q3DInputHandler(parent) {}

  virtual void mousePressEvent(QMouseEvent *event, const QPoint &mousePos) {}
  virtual void mouseMoveEvent(QMouseEvent *event, const QPoint &mousePos) {}
  virtual void mouseReleaseEvent(QMouseEvent *event, const QPoint &mousePos) {}
  virtual void wheelEvent(QWheelEvent *event) {}

private:
  void handleElementSelected(QAbstract3DGraph::ElementType type);
};
