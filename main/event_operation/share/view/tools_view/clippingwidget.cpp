#include "clippingwidget.h"

#include <QBoxLayout>

namespace EventOperation {
ClippingWidget::ClippingWidget(QWidget *parent)
    : QFrame(parent), _clippingSlider(new QSlider(Qt::Horizontal)),
      _clippingLabel(new QLabel("Clipping: 1")) {
  // Setting`s
  setFrameStyle(1);

  _clippingSlider->setMinimumWidth(100);
  _clippingSlider->setTickInterval(1);
  _clippingSlider->setMaximum(100);
  _clippingSlider->setMinimum(0);
  _clippingSlider->setValue(9);
  // Setting`s end

  // Connect`s
  connect(_clippingSlider, &QSlider::valueChanged, [this](int value) {
    float f_value = static_cast<qreal>(value + 1) / 10;
    _clippingLabel->setText(QString("Clipping: %1").arg(f_value));
    emit updateClipping(f_value);
  });
  // Connect`s end

  // Layout`s
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_clippingSlider);
  mainLayout->addWidget(_clippingLabel);

  setLayout(mainLayout);
  // Layout`s end
}

} // namespace EventOperation
