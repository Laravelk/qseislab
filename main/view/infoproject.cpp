#include "infoproject.h"

#include "data/seismproject.h"

#include <QFormLayout>


typedef Data::SeismProject SeismProject;


namespace Main {
InfoProject::InfoProject(QWidget *parent)
    :QFrame(parent),
     _nameLabel(new QLabel(this)),
     _dateLabel(new QLabel(this)),
     _timeLabel(new QLabel(this)),
     _eventsNumber(new QLabel(this)),
     _horizonsNumber(new QLabel(this))
{
    setFrameStyle(1);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Name:", _nameLabel);
    formLayout->addRow("Date:", _dateLabel);
    formLayout->addRow("Time:", _timeLabel);
    formLayout->addRow("Events Number:", _eventsNumber);
    formLayout->addRow("Horizons Number:", _horizonsNumber);

    setLayout(formLayout);
}

void InfoProject::update(const std::unique_ptr<Data::SeismProject>& project)
{
    _nameLabel->setText(project->getName());
    _dateLabel->setText(project->getDateTime().date().toString("dd.MM.yy"));
    _timeLabel->setText(project->getDateTime().time().toString("hh:mm"));
    _eventsNumber->setNum(project->getEventsNumber());
    _horizonsNumber->setNum(project->getHorizonsNumber());
}


} // namespace Main
