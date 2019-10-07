#include "startpage.h"

#include <QBoxLayout>

namespace Main {
StartPage::StartPage(QWidget *parent)
    : QFrame(parent), _buttonNewProject(new QPushButton("New Project", this)),
      _buttonOpenproject(new QPushButton("Open Project", this)) {

  // Setting`s
  setMinimumSize(700, 400); // TODO: remove
  // Setting`s end

  // Connecting
  connect(_buttonNewProject, &QPushButton::clicked,
          [this] { emit newProjectClicked(); });
  connect(_buttonOpenproject, &QPushButton::clicked,
          [this] { emit openProjectClicked(); });
  // Connecting end

  // Layout`s
  QVBoxLayout *buttonLayuot = new QVBoxLayout();
  buttonLayuot->addWidget(_buttonNewProject);
  buttonLayuot->addWidget(_buttonOpenproject);
  //  buttonLayuot->addStretch(1);

  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addLayout(buttonLayuot);
  leftLayout->addStretch(1);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  mainLayout->addStretch(1);

  setLayout(mainLayout);
  // Layout`s end
}

} // namespace Main
