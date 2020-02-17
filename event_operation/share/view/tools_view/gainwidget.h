#pragma once

#include <QFrame>
#include <QLabel>
#include <QSlider>

namespace EventOperation {
class GainWidget : public QFrame {
  Q_OBJECT
public:
  explicit GainWidget(QWidget *parent = nullptr);

signals:
  void updateGain(float) const;

private:
  QSlider *_gainSlider;
  QLabel *_gainLabel;
};

} // namespace EventOperation
