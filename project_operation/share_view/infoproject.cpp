#include "infoproject.h"

#include <QFormLayout>
#include <QDateTime>
//#include <QRegExpValidator>


namespace ProjectOperation {
InfoProject::InfoProject(MODE mode, QWidget* parent)
    :QFrame(parent),
      _nameLineEdit(new QLineEdit(this)),
      _dateLineEdit(new QLineEdit(this)),
      _timeLineEdit(new QLineEdit(this))
{
    setFrameStyle(1);

    // TODO: поставить валидатор на поля ввода
//    _dateLineEdit->setValidator(new QRegExpValidator(QRegExp("")));

    switch(mode) {
        case CLEAN:
            break;
        case DEFAULT:
            _nameLineEdit->setText("Default Name Project");
            _dateLineEdit->setText(QDateTime::currentDateTime().date().toString("dd.MM.yy"));
            _timeLineEdit->setText(QDateTime::currentDateTime().time().toString("hh:mm"));
            break;
    }


    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Name:", _nameLineEdit);
    formLayout->addRow("Date:", _dateLineEdit);
    formLayout->addRow("Time:", _timeLineEdit);

    setLayout(formLayout);
}

void InfoProject::setName(const QString& name)
{
    _nameLineEdit->setText(name);
}

QString InfoProject::getName() const
{
    return _nameLineEdit->text();
}

void InfoProject::setDate(const QDate& date)
{
    _dateLineEdit->setText(date.toString("dd.MM.yy"));
}

QDate InfoProject::getDate() const
{
    return QDate::fromString(_dateLineEdit->text(), "dd.MM.yy");
}

void InfoProject::setTime(const QTime& time)
{
    _timeLineEdit->setText(time.toString("hh:mm"));
}

QTime InfoProject::getTime() const
{
    return QTime::fromString(_timeLineEdit->text(), "hh:mm");
}

void InfoProject::clear()
{
    _nameLineEdit->clear();
    _dateLineEdit->clear();
    _timeLineEdit->clear();
}


} // namespace ProjectOperation
