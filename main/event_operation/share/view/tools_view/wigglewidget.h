#pragma once

#include <QFrame>
#include <QRadioButton>

namespace EventOperation {
class WiggleWidget : public QFrame {
  Q_OBJECT
public:
  enum WiggleState { Negative = -1, None = 0, Positive = 1 };

  explicit WiggleWidget(QWidget *parent = nullptr);

signals:
  void updateWiggleState(WiggleState) const;

private:
  QRadioButton *_noneWiggleButton;
  QRadioButton *_positiveWiggleButton;
  QRadioButton *_negativeWiggleButton;
};

} // namespace EventOperation
