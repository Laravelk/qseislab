#pragma once

#include <QCheckBox>
#include <QFrame>

namespace EventOperation {
class HideComponentWidget : public QFrame {
  Q_OBJECT
public:
  //    enum HideComponentWidget { Negative = -1, None = 0, Positive = 1 };
  enum Axis { X, Y, Z };
  enum State { Unchecked, Checked };

  explicit HideComponentWidget(QWidget *parent = nullptr);

signals:
  //    void updateWiggleState(WiggleState) const;
  void updateAxisState(Axis, State) const;

private:
  //    QRadioButton *_noneWiggleButton;
  //    QRadioButton *_positiveWiggleButton;
  //    QRadioButton *_negativeWiggleButton;
  QCheckBox *_x_axisButton;
  QCheckBox *_y_axisButton;
  QCheckBox *_z_axisButton;
};

} // namespace EventOperation
