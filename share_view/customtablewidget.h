#pragma once

#include <QTableWidget>

class CustomTableWidget : public QTableWidget {
  Q_OBJECT
public:
  explicit CustomTableWidget(QWidget *parent = nullptr);

signals:
  void rightClicked(const QPoint &) const;

protected:
  void mousePressEvent(QMouseEvent *) override;
};
