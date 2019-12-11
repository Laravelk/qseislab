#include "customtablewidget.h"

#include <QMouseEvent>

CustomTableWidget::CustomTableWidget(QWidget *parent) : QTableWidget(parent) {}

void CustomTableWidget::mousePressEvent(QMouseEvent *event) {
  QTableWidget::mousePressEvent(event);

  if (Qt::RightButton == event->button()) {
    if (selectionModel()->hasSelection()) {
      emit rightClicked(event->globalPos());
    }
  }
}
