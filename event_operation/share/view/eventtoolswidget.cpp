#include "eventtoolswidget.h"

<<<<<<< HEAD
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
=======
#include <QBoxLayout>
#include <QPushButton>

namespace EventOperation {
EventToolsWidget::EventToolsWidget(QWidget *parent) : QWidget(parent) {

  // Setting`s
  QPushButton *dataToEBasisButton = new QPushButton("Data to E-basis");
  // Setting`s end

  // Connecting
  connect(dataToEBasisButton, &QPushButton::clicked,
          [this]() { emit dataToEBasisClicked(); });
>>>>>>> test
  // Connecting end

  // Layout`s
  QHBoxLayout *mainLayout = new QHBoxLayout();
<<<<<<< HEAD
  mainLayout->addWidget(_dataToEBasisButton);
  mainLayout->addWidget(_testMultButton);
=======
  mainLayout->addWidget(dataToEBasisButton);
>>>>>>> test

  setLayout(mainLayout);
  // Layout`s end
}

<<<<<<< HEAD
void EventToolsWidget::update(SeismEvent const *const event) {
  _dataToEBasisButton->setDisabled(
      event->isTransformBy(SeismEvent::TransformOperation::RotateDataToEBasis));
  _testMultButton->setDisabled(
      event->isTransformBy(SeismEvent::TransformOperation::TestMultiplier));
}

=======
>>>>>>> test
} // namespace EventOperation
