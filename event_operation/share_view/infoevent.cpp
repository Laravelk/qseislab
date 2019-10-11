#include "infoevent.h"

#include "data/seismevent.h"

#include <QFormLayout>

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
InfoEvent::InfoEvent(QWidget *parent)
    : QFrame(parent), _dateEdit(new QDateEdit(QDate::currentDate(), this)),
      _timeEdit(new QTimeEdit(QTime::currentTime(), this)),
      _traceNumberLabel(new QLabel(this)), _lengthLabel(new QLabel(this)),
      _groupeCoordinate(new QLabel(this)) {

  setFixedWidth(250);
  setFrameStyle(1);

  _dateEdit->setMinimumDate(QDate::currentDate().addDays(-365));
  _dateEdit->setMaximumDate(QDate::currentDate().addDays(365));
  _dateEdit->setDisplayFormat("dd.MM.yy");
  _dateEdit->setCalendarPopup(true);

  QFormLayout *formLayout = new QFormLayout;
  formLayout->addRow("Date:", _dateEdit);
  formLayout->addRow("Time:", _timeEdit);
  formLayout->addRow("Component Number:", _traceNumberLabel);
  formLayout->addRow("Length:", _lengthLabel);
  formLayout->addRow("Groupe Coordinate:", _groupeCoordinate);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(formLayout);

  setLayout(layout);
}

void InfoEvent::setDisabled(bool b) {
  _dateEdit->setDisabled(b);
  _timeEdit->setDisabled(b);
}

void InfoEvent::setEnabled(bool b) {
  _dateEdit->setEnabled(b);
  _timeEdit->setEnabled(b);
}

void InfoEvent::update(const std::unique_ptr<Data::SeismEvent> &event) {
  if (event) {
    _dateEdit->setDate(event->getDateTime().date());
    _timeEdit->setTime(event->getDateTime().time());
    _traceNumberLabel->setText(QString::number(event->getComponentNumber()));
  }
}

void InfoEvent::clear() {
  //  _dateEdit->clear();
  //  _timeEdit->clear();
  _traceNumberLabel->clear();
  _lengthLabel->clear();
  _groupeCoordinate->clear();
}

void InfoEvent::settingEventInfo(
    const std::unique_ptr<Data::SeismEvent> &event) const {
  event->setDateTime({_dateEdit->date(), _timeEdit->time()});
}

} // namespace EventOperation
