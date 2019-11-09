#include "eventtoolswidget.h"

#include "data/seismevent.h"

#include <QBoxLayout>
#include <QPushButton>

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
EventToolsWidget::EventToolsWidget(QWidget *parent) : QWidget(parent),
    _dataToEBasisButton(new QPushButton("Data to E-basis"))
{

  // Setting`s
  // Setting`s end

  // Connecting
  connect(_dataToEBasisButton, &QPushButton::clicked,
          [this]() { emit eventTransformClicked(SeismEvent::TransformOperation::RotateDataToEBasis); });
  // Connecting end

  // Layout`s
  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addWidget(_dataToEBasisButton);

  setLayout(mainLayout);
  // Layout`s end
}

void EventToolsWidget::update(const std::unique_ptr<SeismEvent> &event)
{
    _dataToEBasisButton->setDisabled(event->isTransformBy(SeismEvent::TransformOperation::RotateDataToEBasis));
}

} // namespace EventOperation
