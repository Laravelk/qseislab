#include "startpage.h"

#include <QBoxLayout>
#include <QDateTimeEdit> // TODO: need to remove

namespace Main {
StartPage::StartPage(QWidget *parent)
    : QFrame(parent), _buttonNewProject(new QPushButton("New Project", this)),
      _buttonOpenproject(new QPushButton("Open Project", this)) {
  connect(_buttonNewProject, SIGNAL(clicked()), this,
          SLOT(handleNewProjectClicked()));
  connect(_buttonOpenproject, SIGNAL(clicked()), this,
          SLOT(handleOpenProjectClicked()));

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addStretch(1);
  layout->addWidget(_buttonNewProject);
  layout->addWidget(_buttonOpenproject);
  layout->addStretch(1);

  setLayout(layout);
}

void StartPage::handleNewProjectClicked() { emit newProjectClicked(); }

void StartPage::handleOpenProjectClicked() { emit openProjectClicked(); }

} // namespace Main
