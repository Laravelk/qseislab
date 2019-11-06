#include "infoproject.h"

#include "data/seismproject.h"

#include <QFormLayout>
#include <QPushButton>
#include <QString>

namespace Main {
InfoProject::InfoProject(const std::unique_ptr<Data::SeismProject> &project,
                         QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint) {

  QPushButton *okButton = new QPushButton("Ok");
  connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(okButton);

  QFormLayout *formLayout = new QFormLayout;
  formLayout->addRow("Name:", (new QLabel(project->getName())));
  formLayout->addRow(
      "Date:", new QLabel(project->getDateTime().date().toString("dd.MM.yy")));
  formLayout->addRow(
      "Time:", new QLabel(project->getDateTime().time().toString("hh:mm")));
  formLayout->addRow(
      "Events Amount:",
      new QLabel(QString::number(project->getAmount<Data::SeismEvent>())));
  formLayout->addRow(
      "Horizons Amount:",
      new QLabel(QString::number(project->getAmount<Data::SeismHorizon>())));
  formLayout->addRow(
      "Wells Amount:",
      new QLabel(QString::number(project->getAmount<Data::SeismWell>())));

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addLayout(formLayout);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonsLayout);
  setLayout(mainLayout);
}

} // namespace Main
