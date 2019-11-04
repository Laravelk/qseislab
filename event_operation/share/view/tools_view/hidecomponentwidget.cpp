#include "hidecomponentwidget.h"

#include <QBoxLayout>
#include <QLabel>

namespace EventOperation {
HideComponentWidget::HideComponentWidget(QWidget *parent)
    : QFrame(parent), _x_axisButton(new QCheckBox("X")),
      _y_axisButton(new QCheckBox("Y")), _z_axisButton(new QCheckBox("Z")) {
  // Setting`s
  setFrameStyle(1);

  _x_axisButton->setChecked(true);
  _y_axisButton->setChecked(true);
  _z_axisButton->setChecked(true);
  // Setting`s end

  // Connect`s
  connect(_x_axisButton, &QCheckBox::stateChanged, [this](int state) {
    (0 == state) ? emit updateAxisState(Axis::X, State::Unchecked)
                 : emit updateAxisState(Axis::X, State::Checked);
  });

  connect(_y_axisButton, &QCheckBox::stateChanged, [this](int state) {
    (0 == state) ? emit updateAxisState(Axis::Y, State::Unchecked)
                 : emit updateAxisState(Axis::Y, State::Checked);
  });

  connect(_z_axisButton, &QCheckBox::stateChanged, [this](int state) {
    (0 == state) ? emit updateAxisState(Axis::Z, State::Unchecked)
                 : emit updateAxisState(Axis::Z, State::Checked);
  });
  // Connect`s end

  // Layout`s
  //  QFormLayout *mainLayout = new QFormLayout;
  //  formLayout->addRow("Name:", _nameEdit);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(new QLabel("Components"));
  mainLayout->addWidget(_x_axisButton);
  mainLayout->addWidget(_y_axisButton);
  mainLayout->addWidget(_z_axisButton);

  setLayout(mainLayout);
  // Layout`s end
}

} // namespace EventOperation