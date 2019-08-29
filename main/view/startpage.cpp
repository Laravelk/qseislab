#include "startpage.h"

#include <QBoxLayout>

namespace Main {
StartPage::StartPage(QWidget *parent)
    : QFrame(parent), _buttonNewProject(new QPushButton("New Project", this)),
      _buttonOpenproject(new QPushButton("Open Project", this)) {

  // Connecting
  connect(_buttonNewProject, &QPushButton::clicked,
          [this] { emit newProjectClicked(); });
  connect(_buttonOpenproject, &QPushButton::clicked,
          [this] { emit openProjectClicked(); });
  // Connecting end

  // Layout`s
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addStretch(1);
  layout->addWidget(_buttonNewProject);
  layout->addWidget(_buttonOpenproject);
  layout->addStretch(1);

  setLayout(layout);
  // Layout`s end
}

} // namespace Main
