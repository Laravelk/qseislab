#include "workpage.h"

#include "../surface_view/surface.h"
#include "data/seismproject.h"
#include "infoproject.h"

#include <QDateTime>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismProject SeismProject;
typedef Data::SeismEvent SeismEvent;

using namespace QtDataVisualization;

namespace Main {
WorkPage::WorkPage(QWidget *parent)
    : QFrame(parent), _infoProject(new InfoProject(this)),
      _eventsTable(new QTableWidget(this)), _graph(new Q3DSurface) {
  _surface = new Surface(_graph);
  QWidget *container = QWidget::createWindowContainer(_graph);
  container->setMinimumSize(QSize(400, 300));
  container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  container->setFocusPolicy(Qt::StrongFocus);

  QPushButton *addEventButton = new QPushButton("AddEvent");
  connect(addEventButton, SIGNAL(clicked()), this,
          SLOT(handleAddEventClicked()));

  QPushButton *addHorizonButton = new QPushButton("AddHorizon");
  connect(addHorizonButton, SIGNAL(clicked()), this,
          SLOT(handleAddHorizonClicked()));

  QPushButton *saveProjectButton = new QPushButton("SaveProject");
  connect(saveProjectButton, SIGNAL(clicked()), this,
          SLOT(handleSaveProjectClicked()));

  QPushButton *closeProjectButton = new QPushButton("CloseProject");
  connect(closeProjectButton, SIGNAL(clicked()), this,
          SLOT(handleCloseProjectClicked()));

  initEventsTable(_eventsTable);
  connect(_eventsTable, SIGNAL(cellDoubleClicked(int, int)), this,
          SLOT(handleEventClicked(int, int)));

  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addWidget(addEventButton);
  buttonsLayout->addWidget(addHorizonButton);
  buttonsLayout->addWidget(saveProjectButton);
  buttonsLayout->addWidget(closeProjectButton);
  buttonsLayout->addStretch(1);

  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->addWidget(_infoProject);
  vLayout->addWidget(container);
  vLayout->addStretch(1);
  vLayout->addLayout(buttonsLayout);
  vLayout->addWidget(_eventsTable);

  setLayout(vLayout);
}

void WorkPage::loadProject(const std::unique_ptr<Data::SeismProject> &project) {
  _infoProject->update(project);
  _surface->setProject(project);
  clearTable();

  for (auto &itr : project->getEventsMap()) {
    insertEventInTable(itr.second);
  }
}

void WorkPage::updateProject(
    const std::unique_ptr<Data::SeismEvent> &event) // event
{
  _infoProject->addEvent();
  _surface->addEvent(event);

  insertEventInTable(event);
}

void WorkPage::updateProjectRemoveEvent(const Data::SeismEvent::Uuid &uuid) {
  _infoProject->removeEvent();
  _surface->removeEvent(uuid);
  removeEventInTable(uuid);
}

void WorkPage::updateProject(
    const std::unique_ptr<Data::SeismHorizon> & /*horizon*/) {
  _infoProject->addHorizon();
}

void WorkPage::updateProjectRemoveHorizon(
    const Data::SeismHorizon::Uuid & /*uuid*/) {
  _infoProject->removeHorizon();
}

void WorkPage::handleAddEventClicked() { emit addEventClicked(); }

void WorkPage::handleEventClicked(int row, int /*col*/) {
  Data::SeismEvent::Uuid uuid = _eventsTable->item(row, 0)->text();

  QMessageBox *msgBox = new QMessageBox(this);
  msgBox->setStandardButtons(QMessageBox::Cancel);
  QPushButton *viewButton = msgBox->addButton("View", QMessageBox::NoRole);
  msgBox->setDefaultButton(viewButton);
  QPushButton *removeButton = msgBox->addButton("Remove", QMessageBox::NoRole);

  msgBox->exec();
  if (msgBox->clickedButton() == viewButton) {
    emit viewEventClicked(uuid);
  } else if (msgBox->clickedButton() == removeButton) {
    emit removeEventClicked(uuid);
  }
}

void WorkPage::handleAddHorizonClicked() { emit addHorizonClicked(); }

void WorkPage::handleSaveProjectClicked() { emit saveProjectClicked(); }

void WorkPage::handleCloseProjectClicked() { emit closeProjectClicked(); }

void WorkPage::clearTable() {
  int end = _eventsTable->rowCount();
  for (int i = 0; i < end; ++i) {
    _eventsTable->removeRow(0);
  }
}

void WorkPage::initEventsTable(QTableWidget *table) {
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setColumnCount(5);

  // configure column settings
  table->setHorizontalHeaderItem(
      0, new QTableWidgetItem("id")); // for editing or removing events
  table->setColumnHidden(0, true);
  //    table->setColumnWidth(0,50);

  table->setHorizontalHeaderItem(1, new QTableWidgetItem("Component Number"));
  table->setColumnWidth(1, 150);
  table->setHorizontalHeaderItem(2, new QTableWidgetItem("Type"));
  table->setColumnWidth(2, 100);
  table->setHorizontalHeaderItem(3, new QTableWidgetItem("Date"));
  table->setColumnWidth(3, 100);
  table->setHorizontalHeaderItem(4, new QTableWidgetItem("Time"));
  table->setColumnWidth(4, 100);
}

void WorkPage::insertEventInTable(
    const std::unique_ptr<Data::SeismEvent> &event) {
  _eventsTable->insertRow(_eventsTable->rowCount());

  _eventsTable->setItem(_eventsTable->rowCount() - 1, 0,
                        new QTableWidgetItem(event->getUuid().toString()));
  _eventsTable->setItem(
      _eventsTable->rowCount() - 1, 1,
      new QTableWidgetItem(QString::number(event->getComponentNumber())));
  //        _eventsTable->setItem(_eventsTable->rowCount()-1, 2, new
  //        QTableWidgetItem());
  _eventsTable->setItem(
      _eventsTable->rowCount() - 1, 3,
      new QTableWidgetItem(event->getDateTime().date().toString("dd.MM.yy")));
  _eventsTable->setItem(
      _eventsTable->rowCount() - 1, 4,
      new QTableWidgetItem(event->getDateTime().time().toString("hh:mm")));
}

void WorkPage::removeEventInTable(const Data::SeismEvent::Uuid &uuid) {
  const QString str_uuid = uuid.toString();
  for (int row = 0; row < _eventsTable->rowCount(); ++row) {
    if (str_uuid == _eventsTable->takeItem(row, 0)->text()) {
      _eventsTable->removeRow(row);
      return;
    }
  }
}

} // namespace Main
