#include "wigglewidget.h"

#include <QBoxLayout>

namespace EventOperation {
WiggleWidget::WiggleWidget(QWidget *parent)
    : QFrame(parent), _noneWiggleButton(new QRadioButton("None Wiggle")),
      _positiveWiggleButton(new QRadioButton("Positive Wiggle")),
      _negativeWiggleButton(new QRadioButton("Negative Wiggle")) {
  // Setting`s
  setFrameStyle(1);
  _noneWiggleButton->setChecked(true);
  // Setting`s end

  // Connect`s
  connect(_noneWiggleButton, &QRadioButton::clicked,
          [this]() { emit updateWiggleState(WiggleState::None); });
  connect(_positiveWiggleButton, &QRadioButton::clicked,
          [this]() { emit updateWiggleState(WiggleState::Positive); });
  connect(_negativeWiggleButton, &QRadioButton::clicked,
          [this]() { emit updateWiggleState(WiggleState::Negative); });
  // Connect`s end

  // Layout`s
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_noneWiggleButton);
  mainLayout->addWidget(_positiveWiggleButton);
  mainLayout->addWidget(_negativeWiggleButton);

  setLayout(mainLayout);
  // Layout`s end
}

} // namespace EventOperation
