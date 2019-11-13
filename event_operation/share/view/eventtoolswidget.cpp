#include "eventtoolswidget.h"

#include "data/seismevent.h"

#include <QBoxLayout>
#include <QPushButton>

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
EventToolsWidget::EventToolsWidget(QWidget *parent)
    : QWidget(parent), _dataToEBasisButton(new QPushButton("Data to E-basis")),
      _testMultButton(new QPushButton("Test-Mult")) {

  // Setting`s
  // Setting`s end

  // Connecting
  connect(_dataToEBasisButton, &QPushButton::clicked, [this]() {
    emit eventTransformClicked(
        SeismEvent::TransformOperation::RotateDataToEBasis);
  });
  connect(_testMultButton, &QPushButton::clicked, [this]() {
    emit eventTransformClicked(SeismEvent::TransformOperation::TestMultiplier);
  });
  // Connecting end

  // Layout`s
  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addWidget(_dataToEBasisButton);
  mainLayout->addWidget(_testMultButton);

  setLayout(mainLayout);
  // Layout`s end
}

void EventToolsWidget::update(const std::shared_ptr<SeismEvent> &event) {
  _dataToEBasisButton->setDisabled(
      event->isTransformBy(SeismEvent::TransformOperation::RotateDataToEBasis));
  _testMultButton->setDisabled(
      event->isTransformBy(SeismEvent::TransformOperation::TestMultiplier));
}

} // namespace EventOperation
