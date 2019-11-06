#pragma once

#include <QWidget>

namespace EventOperation {
class EventToolsWidget : public QWidget {
  Q_OBJECT

public:
  explicit EventToolsWidget(QWidget *parent = nullptr);

signals:
  void dataToEBasisClicked() const;
};
} // namespace EventOperation
