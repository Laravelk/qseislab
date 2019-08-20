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
  setWindowTitle("Create New Project");

  connect(_createButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(_createButton);
  buttonLayout->addWidget(_cancelButton);

  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->addWidget(_infoProject);
  vLayout->addStretch(1);
  vLayout->addLayout(buttonLayout);

  setLayout(vLayout);
}

void View::settingProjectInfo(
    const std::unique_ptr<Data::SeismProject> &project) {
  _infoProject->settingProjectInfo(project);
}

} // namespace NewProject
} // namespace ProjectOperation
