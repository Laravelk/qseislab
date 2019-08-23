#include "infoevent.h"

#include "data/seismevent.h"

#include <QFormLayout>

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
InfoEvent::InfoEvent(QWidget *parent)
    : QFrame(parent), _dateLineEdit(new QLineEdit(this)),
      _timeLineEdit(new QLineEdit(this)), _traceNumberLabel(new QLabel(this)),
      _lengthLabel(new QLabel(this)), _groupeCoordinate(new QLabel(this)) {
  setFixedWidth(250);
  setFrameStyle(1);

  // TODO: поставить валидатор на поля ввода

  QFormLayout *formLayout = new QFormLayout;
  formLayout->addRow("Date:", _dateLineEdit);
  formLayout->addRow("Time:", _timeLineEdit);
  formLayout->addRow("Component Number:", _traceNumberLabel);
  formLayout->addRow("Length:", _lengthLabel);
  formLayout->addRow("Groupe Coordinate:", _groupeCoordinate);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(formLayout);

  setLayout(layout);
}

void InfoEvent::setDisabled(bool b) {
  _dateLineEdit->setDisabled(b);
  _timeLineEdit->setDisabled(b);
}

void InfoEvent::setEnabled(bool b) {
  _dateLineEdit->setEnabled(b);
  _timeLineEdit->setEnabled(b);
}

void InfoEvent::update(const std::unique_ptr<Data::SeismEvent> &event) {
  if (event) {
    _dateLineEdit->setText(event->getDateTime().date().toString("dd.MM.yy"));
    _timeLineEdit->setText(event->getDateTime().time().toString("hh:mm"));
    _traceNumberLabel->setText(QString::number(event->getComponentNumber()));
  }
}

void InfoEvent::clear() {
  _dateLineEdit->clear();
  _timeLineEdit->clear();
  _traceNumberLabel->clear();
  _lengthLabel->clear();
  _groupeCoordinate->clear();
}

void InfoEvent::settingEventInfo(
    const std::unique_ptr<Data::SeismEvent> &event) const {
  QDate date = QDate::fromString(_dateLineEdit->text(), "dd.MM.yy");
  QTime time = QTime::fromString(_timeLineEdit->text(), "hh:mm");
  event->setDateTime(QDateTime(date, time));
}

} // namespace EventOperation