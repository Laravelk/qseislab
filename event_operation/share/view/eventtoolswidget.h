#pragma once

#include "data/seismevent.h"

#include <QWidget>

#include <QPushButton>
#include <memory>

namespace EventOperation {
class EventToolsWidget : public QWidget {
  Q_OBJECT

public:
  explicit EventToolsWidget(QWidget *parent = nullptr);

  void update(const std::shared_ptr<Data::SeismEvent> &);

signals:
  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;
  //  void dataToEBasisClicked() const;

private:
  QPushButton *_dataToEBasisButton;
  QPushButton *_testMultButton;
};
} // namespace EventOperation
