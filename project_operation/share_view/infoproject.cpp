#include "infoproject.h"

#include "data/seismproject.h"

#include <QDateTime>
#include <QFormLayout>
//#include <QRegExpValidator>

namespace ProjectOperation {
InfoProject::InfoProject(MODE mode, QWidget *parent)
    : QFrame(parent), _nameLineEdit(new QLineEdit(this)),
      _dateLineEdit(new QLineEdit(this)), _timeLineEdit(new QLineEdit(this)) {
  setFrameStyle(1);

  // TODO: поставить валидатор на поля ввода
  //    _dateLineEdit->setValidator(new QRegExpValidator(QRegExp("")));

  switch (mode) {
  case CLEAN:
    break;
  case DEFAULT:
    _nameLineEdit->setText("Default Name Project");
    _dateLineEdit->setText(
        QDateTime::currentDateTime().date().toString("dd.MM.yy"));
    _timeLineEdit->setText(
        QDateTime::currentDateTime().time().toString("hh:mm"));
    break;
  }

  QFormLayout *formLayout = new QFormLayout;
  formLayout->addRow("Name:", _nameLineEdit);
  formLayout->addRow("Date:", _dateLineEdit);
  formLayout->addRow("Time:", _timeLineEdit);

  setLayout(formLayout);
}

void InfoProject::setDisabled(bool b) {
  _nameLineEdit->setDisabled(b);
  _dateLineEdit->setDisabled(b);
  _timeLineEdit->setDisabled(b);
}

void InfoProject::setEnabled(bool b) {
  _nameLineEdit->setEnabled(b);
  _dateLineEdit->setEnabled(b);
  _timeLineEdit->setEnabled(b);
}

void InfoProject::update(const std::unique_ptr<Data::SeismProject> &project) {
  if (!project) {
    setDisabled(true);
  } else {
    _nameLineEdit->setText(project->getName());
    _dateLineEdit->setText(project->getDateTime().date().toString("dd.MM.yy"));
    _timeLineEdit->setText(project->getDateTime().time().toString("hh:mm"));
    setDisabled(false);
  }
}

void InfoProject::settingProjectInfo(
    const std::unique_ptr<Data::SeismProject> &project) {
  project->setName(_nameLineEdit->text());
  project->setDate(QDate::fromString(_dateLineEdit->text(), "dd.MM.yy"));
  project->setTime(QTime::fromString(_timeLineEdit->text(), "hh:mm"));
}

void InfoProject::clear() {
  _nameLineEdit->clear();
  _dateLineEdit->clear();
  _timeLineEdit->clear();
}

} // namespace ProjectOperation
