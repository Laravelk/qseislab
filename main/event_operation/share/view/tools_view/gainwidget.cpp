#include "gainwidget.h"

#include <QBoxLayout>

namespace EventOperation {
GainWidget::GainWidget(QWidget *parent)
    : QFrame(parent), _gainSlider(new QSlider(Qt::Horizontal)),
      _gainLabel(new QLabel("Gain: 1")) {
  // Setting`s
  setFrameStyle(1);

  _gainSlider->setTickInterval(1);
  _gainSlider->setMaximum(100);
  _gainSlider->setMinimum(0);
  _gainSlider->setValue(9);
  // Setting`s end

  // Connect`s
  connect(_gainSlider, &QSlider::valueChanged, [this](int value) {
    float f_value = static_cast<qreal>(value + 1) / 10;
    _gainLabel->setText(QString("Gain: %1").arg(f_value));
    emit updateGain(f_value);
  });
  // Connect`s end

  // Layout`s
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_gainSlider);
  mainLayout->addWidget(_gainLabel);

  setLayout(mainLayout);
  // Layout`s end
}

} // namespace EventOperation
