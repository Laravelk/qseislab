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
      //      _receiversNumber(new QLabel(this)),
      _wellsNumber(new QLabel(this)) {

  setFrameStyle(1);

  QFormLayout *formLayout = new QFormLayout;
  formLayout->addRow("Name:", _nameLabel);
  formLayout->addRow("Date:", _dateLabel);
  formLayout->addRow("Time:", _timeLabel);
  formLayout->addRow("Events Number:", _eventsNumber);
  formLayout->addRow("Horizons Number:", _horizonsNumber);
  //  formLayout->addRow("Receivers Number:", _receiversNumber);
  formLayout->addRow("Wells Number:", _wellsNumber);

  setLayout(formLayout);
}

void InfoProject::update(const std::unique_ptr<Data::SeismProject> &project) {
  _nameLabel->setText(project->getName());
  _dateLabel->setText(project->getDateTime().date().toString("dd.MM.yy"));
  _timeLabel->setText(project->getDateTime().time().toString("hh:mm"));
  _eventsNumber->setNum(project->getNumber<Data::SeismEvent>());
  _horizonsNumber->setNum(project->getNumber<Data::SeismHorizon>());
  //  _receiversNumber->setNum(project->getNumber<Data::SeismReceiver>());
  _wellsNumber->setNum(project->getNumber<Data::SeismWell>());
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

// void InfoProject::addReceiver() {
//  _receiversNumber->setNum(_receiversNumber->text().toInt() + 1);
//}

// void InfoProject::removeReceiver() {
//  _receiversNumber->setNum(_receiversNumber->text().toInt() - 1);
//}

void InfoProject::addWell() {
  _wellsNumber->setNum(_wellsNumber->text().toInt() + 1);
}

void InfoProject::removeWell() {
  _wellsNumber->setNum(_wellsNumber->text().toInt() - 1);
}

} // namespace Main
