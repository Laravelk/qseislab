#include "infoproject.h"

#include "data/seismproject.h"

#include <QDateTime>
#include <QFormLayout>

namespace ProjectOperation {
InfoProject::InfoProject(MODE mode, QWidget *parent)
    : QFrame(parent), _nameLineEdit(new QLineEdit(this)),
      _dateEdit(new QDateEdit(QDate::currentDate(), this)),
      _timeEdit(new QTimeEdit(QTime::currentTime(), this)) {
  setFrameStyle(1);

  _dateEdit->setMinimumDate(QDate::currentDate().addDays(-365));
  _dateEdit->setMaximumDate(QDate::currentDate().addDays(365));
  _dateEdit->setDisplayFormat("dd.MM.yy");
  _dateEdit->setCalendarPopup(true);

  switch (mode) {
  case CLEAN:
    break;
  case DEFAULT:
    _nameLineEdit->setText("Default Name Project");
    break;
  }

  QFormLayout *formLayout = new QFormLayout();
  formLayout->addRow("Name: ", _nameLineEdit);
  formLayout->addRow("Date: ", _dateEdit);
  formLayout->addRow("Time: ", _timeEdit);

  setLayout(formLayout);
}

void InfoProject::setDisabled(bool b) {
  _nameLineEdit->setDisabled(b);
  _dateEdit->setDisabled(b);
  _timeEdit->setDisabled(b);
}

void InfoProject::setEnabled(bool b) {
  _nameLineEdit->setEnabled(b);
  _dateEdit->setEnabled(b);
  _timeEdit->setEnabled(b);
}

void InfoProject::update(const std::unique_ptr<Data::SeismProject> &project) {
  if (!project) {
    setDisabled(true);
  } else {
    _nameLineEdit->setText(project->getName());
    _dateEdit->setDate(project->getDateTime().date());
    _timeEdit->setTime(project->getDateTime().time());
    setDisabled(false);
  }
}

void InfoProject::settingProjectInfo(
    const std::unique_ptr<Data::SeismProject> &project) {
  project->setName(_nameLineEdit->text());
  project->setDate(_dateEdit->date());
  project->setTime(_timeEdit->time());
}

void InfoProject::clear() {
  _nameLineEdit->clear();
  _dateEdit->clear();
  _timeEdit->clear();
}

} // namespace ProjectOperation
