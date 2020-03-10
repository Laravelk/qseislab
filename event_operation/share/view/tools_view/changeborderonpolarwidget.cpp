#include "changeborderonpolarwidget.h"

#include <QIntValidator>
#include <iostream>

namespace EventOperation {

ChangeBorderOnPolarWidget::ChangeBorderOnPolarWidget(QWidget *parent)
    : QFrame(parent), _angularMin(new QLineEdit), _angularMax(new QLineEdit),
      _radialMin(new QLineEdit), _radialMax(new QLineEdit),
      _angularLabel(new QLabel), _minAngularLabel(new QLabel),
      _maxAngularLabel(new QLabel), _radialLabel(new QLabel),
      _minRadialLabel(new QLabel), _maxRadialLabel(new QLabel),
      _applyButton(new QPushButton("Apply Change")) {
  this->setMaximumSize(250, 250);
  QVBoxLayout *mainLayout = new QVBoxLayout();
  QHBoxLayout *angularLayout = new QHBoxLayout();
  QHBoxLayout *radialLayout = new QHBoxLayout();

  _angularLabel->setText("Angular border");
  _minAngularLabel->setText("Min");
  _maxAngularLabel->setText("Max");

  _radialLabel->setText("Radial border");
  _minRadialLabel->setText("Min");
  _maxRadialLabel->setText("Max");

  _angularMin->setValidator(new QIntValidator(INT_MIN, INT_MAX, this));
  _angularMax->setValidator(new QIntValidator(INT_MIN, INT_MAX, this));

  _radialMin->setValidator(new QIntValidator(INT_MIN, INT_MAX, this));
  _radialMax->setValidator(new QIntValidator(INT_MIN, INT_MAX, this));

  _angularMin->setText("0");
  _angularMax->setText("360");

  _radialMin->setText("0");
  _radialMax->setText("180");

  angularLayout->addWidget(_minAngularLabel);
  angularLayout->addWidget(_angularMin);
  angularLayout->addWidget(_maxAngularLabel);
  angularLayout->addWidget(_angularMax);

  radialLayout->addWidget(_minRadialLabel);
  radialLayout->addWidget(_radialMin);
  radialLayout->addWidget(_maxRadialLabel);
  radialLayout->addWidget(_radialMax);

  mainLayout->addWidget(_angularLabel);
  mainLayout->addLayout(angularLayout);
  mainLayout->addWidget(_radialLabel);
  mainLayout->addLayout(radialLayout);
  mainLayout->addWidget(_applyButton);

  connect(_applyButton, &QPushButton::clicked, [this]() {
    if (_radialMaxChanged || _radialMinChanged || _angularMinChanged ||
        _angularMaxChanged) {
      _radialMinChanged = false;
      _radialMaxChanged = false;
      _angularMinChanged = false;
      _angularMaxChanged = false;
      emit valueChanged();
    }
  });

  connect(_radialMin, &QLineEdit::textChanged,
          [this]() { _radialMinChanged = true; });

  connect(_radialMax, &QLineEdit::textChanged,
          [this]() { _radialMaxChanged = true; });

  connect(_angularMin, &QLineEdit::textChanged,
          [this]() { _angularMinChanged = true; });

  connect(_angularMax, &QLineEdit::textChanged,
          [this]() { _angularMaxChanged = true; });

  this->setLayout(mainLayout);
  setFrameStyle(1);
}

int ChangeBorderOnPolarWidget::getAngularMin() {
  return _angularMin->text().toInt();
}

int ChangeBorderOnPolarWidget::getAngularMax() {
  return _angularMax->text().toInt();
}

int ChangeBorderOnPolarWidget::getRadialMin() {
  return _radialMin->text().toInt();
}

int ChangeBorderOnPolarWidget::getRadialMax() {
  return _radialMax->text().toInt();
}
}
