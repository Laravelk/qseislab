#include "infoproject.h"

#include "data/seismproject.h"

#include <QFormLayout>
#include <QString>

typedef Data::SeismProject SeismProject;

namespace Main {
InfoProject::InfoProject(QWidget *parent)
    : QFrame(parent), _nameLabel(new QLabel(this)),
      _dateLabel(new QLabel(this)), _timeLabel(new QLabel(this)),
      _eventsNumber(new QLabel(this)), _horizonsNumber(new QLabel(this)),
      _recieversNumber(new QLabel(this)) {

  setFrameStyle(1);

  QFormLayout *formLayout = new QFormLayout;
  formLayout->addRow("Name:", _nameLabel);
  formLayout->addRow("Date:", _dateLabel);
  formLayout->addRow("Time:", _timeLabel);
  formLayout->addRow("Events Number:", _eventsNumber);
  formLayout->addRow("Horizons Number:", _horizonsNumber);
  formLayout->addRow("Recievers Number:", _recieversNumber);

  setLayout(formLayout);
}

void InfoProject::update(const std::unique_ptr<Data::SeismProject> &project) {
  _nameLabel->setText(project->getName());
  _dateLabel->setText(project->getDateTime().date().toString("dd.MM.yy"));
  _timeLabel->setText(project->getDateTime().time().toString("hh:mm"));
  _eventsNumber->setNum(project->getNumber<Data::SeismEvent>());
  _horizonsNumber->setNum(project->getNumber<Data::SeismHorizon>());
  _recieversNumber->setNum(project->getNumber<Data::SeismReciever>());
}

void InfoProject::addEvent() {
  _eventsNumber->setNum(_eventsNumber->text().toInt() + 1);
}

void InfoProject::removeEvent() {
  _eventsNumber->setNum(_eventsNumber->text().toInt() - 1);
}

void InfoProject::addHorizon() {
  _horizonsNumber->setNum(_horizonsNumber->text().toInt() + 1);
}

void InfoProject::removeHorizon() {
  _horizonsNumber->setNum(_horizonsNumber->text().toInt() - 1);
}

void InfoProject::addReciever() {
  _recieversNumber->setNum(_recieversNumber->text().toInt() + 1);
}

void InfoProject::removeReciever() {
  _recieversNumber->setNum(_recieversNumber->text().toInt() - 1);
}

} // namespace Main
