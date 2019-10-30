#include "infoevent.h"

#include "data/seismevent.h"

#include <QFormLayout>

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
InfoEvent::InfoEvent(QWidget *parent)
    : QFrame(parent), _nameEdit(new QLineEdit()),
      _stampDateEdit(new QDateEdit()), _stampTimeEdit(new QTimeEdit()),
      _receiverAmountLabel(new QLabel()), _pWavePickAmountLabel(new QLabel()),
      _sWavePickAmountLabel(new QLabel()), _addedDateLabel(new QLabel()),
      _addedTimeLabel(new QLabel()) {

  setFixedWidth(250);
  setFrameStyle(1);

  _stampDateEdit->setMinimumDate(QDate::currentDate().addDays(-365));
  _stampDateEdit->setMaximumDate(QDate::currentDate().addDays(365));
  _stampDateEdit->setDisplayFormat("dd.MM.yy");
  _stampDateEdit->setCalendarPopup(true);

  _stampTimeEdit->setDisplayFormat("hh:mm:zzz");

  connect(_nameEdit, &QLineEdit::textChanged,
          [this](auto &text) { emit nameChanged(text); });

  QFormLayout *formLayout = new QFormLayout;
  formLayout->addRow("Name:", _nameEdit);
  formLayout->addRow("Stamp date:", _stampDateEdit);
  formLayout->addRow("Stamp time:", _stampTimeEdit);
  formLayout->addRow("Receiver Amount:", _receiverAmountLabel);
  formLayout->addRow("P-Wave-Pick Amount:", _pWavePickAmountLabel);
  formLayout->addRow("S-Wave-Pick Amount:", _sWavePickAmountLabel);
  formLayout->addRow("Added date:", _addedDateLabel);
  formLayout->addRow("Added time:", _addedTimeLabel);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(formLayout);

  setLayout(layout);
}

void InfoEvent::setDisabled(bool b) {
  _nameEdit->setDisabled(b);
  _stampDateEdit->setDisabled(b);
  _stampTimeEdit->setDisabled(b);
}

void InfoEvent::setEnabled(bool b) {
  _nameEdit->setEnabled(b);
  _stampDateEdit->setEnabled(b);
  _stampTimeEdit->setEnabled(b);
}

void InfoEvent::update(const std::unique_ptr<Data::SeismEvent> &event) {
  if (event) {
    _nameEdit->setText(event->getName());
    _stampDateEdit->setDate(event->getStampDateTime().date());
    _stampTimeEdit->setTime(event->getStampDateTime().time());
    _receiverAmountLabel->setText(QString::number(event->getComponentAmount()));
    _pWavePickAmountLabel->setText(QString::number(
        event->getPickAmountByType(Data::SeismWavePick::Type::PWAVE)));
    _sWavePickAmountLabel->setText(QString::number(
        event->getPickAmountByType(Data::SeismWavePick::Type::SWAVE)));
    _addedDateLabel->setText(
        event->getAddedDateTime().date().toString("dd.MM.yy"));
    _addedTimeLabel->setText(
        event->getAddedDateTime().time().toString("hh:mm"));
  }
}

void InfoEvent::clear() {
  _nameEdit->clear();
  _receiverAmountLabel->clear();
  _pWavePickAmountLabel->clear();
  _sWavePickAmountLabel->clear();
  _addedDateLabel->clear();
  _addedTimeLabel->clear();
}

void InfoEvent::settingEventInfo(
    const std::unique_ptr<Data::SeismEvent> &event) const {
  event->setName(_nameEdit->text());
  event->setStampDateTime({_stampDateEdit->date(), _stampTimeEdit->time()});
}

} // namespace EventOperation
