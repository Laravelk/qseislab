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

  //  setFixedWidth(250);
  setFrameStyle(1);

  _stampDateEdit->setMinimumDate(QDate::currentDate().addDays(-365));
  _stampDateEdit->setMaximumDate(QDate::currentDate().addDays(365));
  _stampDateEdit->setDisplayFormat("dd.MM.yy");
  _stampDateEdit->setCalendarPopup(true);

  _stampTimeEdit->setDisplayFormat("hh:mm:zzz");

  connect(_nameEdit, &QLineEdit::textChanged, [this](auto &text) {
    if (0 != text.count()) {
      emit nameChanged(text);
    }
  });

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

void InfoEvent::setBrush(const QBrush &brush) {
  QPalette palette;
  palette.setBrush(QPalette::Base, brush);
  _nameEdit->setPalette(palette);
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

void InfoEvent::update(SeismEvent const *const event) {
  if (event) {
      auto& eventInfo = event->getInfo();
    _nameEdit->setText(eventInfo.getName());
    _stampDateEdit->setDate(eventInfo.getStampDateTime().date());
    _stampTimeEdit->setTime(eventInfo.getStampDateTime().time());
    _receiverAmountLabel->setText(QString::number(event->getComponentAmount()));
    _pWavePickAmountLabel->setText(QString::number(
        event->getPickAmountByType(Data::SeismWavePick::Type::PWAVE)));
    _sWavePickAmountLabel->setText(QString::number(
        event->getPickAmountByType(Data::SeismWavePick::Type::SWAVE)));
    _addedDateLabel->setText(
        eventInfo.getAddedDateTime().date().toString("dd.MM.yy"));
    _addedTimeLabel->setText(
        eventInfo.getAddedDateTime().time().toString("hh:mm"));
  }
}

void InfoEvent::clear() {
  _nameEdit->clear();
  QPalette palette;
  palette.setBrush(QPalette::Base, Qt::white);
  _nameEdit->setPalette(palette);

  _receiverAmountLabel->clear();
  _pWavePickAmountLabel->clear();
  _sWavePickAmountLabel->clear();
  _addedDateLabel->clear();
  _addedTimeLabel->clear();
}

void InfoEvent::settingEventInfo(SeismEvent *const event) const {
    auto& info = event->getInfo();
  info.setName(_nameEdit->text());
  info.setStampDateTime({_stampDateEdit->date(), _stampTimeEdit->time()});

  event->setInfo(info);
}

} // namespace EventOperation
