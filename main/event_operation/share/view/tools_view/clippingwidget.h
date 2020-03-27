#pragma once

#include <QFrame>
#include <QLabel>
#include <QSlider>

namespace EventOperation {
class ClippingWidget : public QFrame {
  Q_OBJECT
public:
  explicit ClippingWidget(QWidget *parent = nullptr);

signals:
  void updateClipping(float) const;

private:
  QSlider *_clippingSlider;
  QLabel *_clippingLabel;
};

} // namespace EventOperation
