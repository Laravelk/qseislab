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

namespace Main {
WorkPage::WorkPage(QWidget *parent)
    : QFrame(parent), _infoProject(new InfoProject(this)),
      _eventsTable(new QTableWidget(this)), _graph(new Q3DSurface) {
  initEventsTable(_eventsTable);
  _surface = new Surface(_graph);
  QWidget *container = QWidget::createWindowContainer(_graph);
  container->setMinimumSize(QSize(400, 300));
  container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  container->setFocusPolicy(Qt::StrongFocus);
  connect(_eventsTable, SIGNAL(cellDoubleClicked(int, int)), this,
          SLOT(handleEventClicked(int, int)));

  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->addWidget(_infoProject);
  vLayout->addWidget(container);
  vLayout->addStretch(1);
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

void WorkPage::updateProject(const std::unique_ptr<Data::SeismEvent> &event) {
  _infoProject->addEvent();
  _surface->addEvent(event);

  insertEventInTable(event);
}

void WorkPage::updateProjectRemoveEvent(const QUuid &uuid) {
  _infoProject->removeEvent();
  _surface->removeEvent(uuid);

  removeEventInTable(uuid);
}

void WorkPage::updateProject(
    const std::unique_ptr<Data::SeismHorizon> &horizon) {
  _infoProject->addHorizon();
  _surface->addHorizon(horizon);
}

void WorkPage::updateProjectRemoveHorizon(const QUuid & /*uuid*/) {
  _infoProject->removeHorizon();
}

void WorkPage::handleEventClicked(int row, int col) {
  QUuid uuid = _eventsTable->item(row, 0)->text();

  if (5 == col) {
    emit removeEventClicked(uuid);
    return;
  }

  emit viewEventClicked(uuid);
}

void WorkPage::clearTable() {
  int end = _eventsTable->rowCount();
  for (int i = 0; i < end; ++i) {
    _eventsTable->removeRow(0);
  }
}

void WorkPage::initEventsTable(QTableWidget *table) {
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table->setColumnCount(6);

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
  table->setHorizontalHeaderItem(5, new QTableWidgetItem("Remove"));
  table->setColumnWidth(5, 50);
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

  //  QPushButton *removeButton = new QPushButton("Remove");
  //    connect(removeButton, SIGNAL(clicked()), )
  QTableWidgetItem *removeItem = new QTableWidgetItem("Remove");
  removeItem->setTextAlignment(Qt::AlignCenter);
  removeItem->setBackground(Qt::red);
  _eventsTable->setItem(_eventsTable->rowCount() - 1, 5, removeItem);
}

void WorkPage::removeEventInTable(const QUuid &uuid) {
  const QString str_uuid = uuid.toString();
  for (int row = 0; row < _eventsTable->rowCount(); ++row) {
    if (str_uuid == _eventsTable->takeItem(row, 0)->text()) {
      _eventsTable->removeRow(row);
      return;
    }
  }
}

} // namespace Main
