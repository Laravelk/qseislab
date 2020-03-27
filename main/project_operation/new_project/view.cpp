#include "view.h"

#include "data/seismproject.h"
#include "project_operation/share_view/infoproject.h"

#include <QVBoxLayout>

namespace ProjectOperation {
namespace NewProject {
View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _infoProject(new InfoProject(InfoProject::DEFAULT, this)),
      _createButton(new QPushButton("Create", this)),
      _cancelButton(new QPushButton("Cancel", this)) {

  // Setting`s
  setWindowTitle("Create New Project");
  // Setting`s end

  // Connecting
  connect(_createButton, &QPushButton::clicked, this, &View::accept);
  connect(_cancelButton, &QPushButton::clicked, this, &View::reject);
  // Connecting end

  // Layout`s
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(_createButton);
  buttonLayout->addWidget(_cancelButton);

  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->addWidget(_infoProject);
  vLayout->addStretch(1);
  vLayout->addLayout(buttonLayout);

  setLayout(vLayout);
  // Layout`s end
}

void View::settingProjectInfo(Data::SeismProject *const project) {
  _infoProject->settingProjectInfo(project);
}

} // namespace NewProject
} // namespace ProjectOperation
