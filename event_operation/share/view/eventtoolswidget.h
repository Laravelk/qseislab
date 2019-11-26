#pragma once

<<<<<<< HEAD
#include "data/seismevent.h"

#include <QWidget>

#include <QPushButton>
#include <memory>

=======
#include <QWidget>

>>>>>>> test
namespace EventOperation {
class EventToolsWidget : public QWidget {
  Q_OBJECT

public:
  explicit EventToolsWidget(QWidget *parent = nullptr);

<<<<<<< HEAD
  void update(Data::SeismEvent const *const);

signals:
  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;
  //  void dataToEBasisClicked() const;

private:
  QPushButton *_dataToEBasisButton;
  QPushButton *_testMultButton;
=======
signals:
  void dataToEBasisClicked() const;
>>>>>>> test
};
} // namespace EventOperation
