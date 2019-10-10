#include "workpage.h"

#include "../surface_view/surface.h"
#include "data/seismhorizon.h"
#include "data/seismproject.h"
#include "data/seismreceiver.h"
#include "infoproject.h"

#include <iostream> // TODO: delete

#include <QDateTime>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>

typedef Data::SeismProject SeismProject;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismReceiver SeismReceiver;

namespace Main {
WorkPage::WorkPage(QWidget *parent)
    : QFrame(parent), _infoProject(new InfoProject(this)),
      //      _eventsTable(new QTableWidget(this)),
      _eventsTable(new TableAssistant(TableAssistant::ForEvents)),
      _graph(new Q3DSurface) {

  // Setting`s
  //  initEventsTable(_eventsTable);
  _graph->setMinimumWidth(400);
  _graph->setMinimumHeight(700);
  _surface = new Surface(_graph);
  QWidget *container = QWidget::createWindowContainer(_graph);
  container->setMinimumSize(QSize(400, 400));
  container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  container->setFocusPolicy(Qt::StrongFocus);
  // Setting`s end

  // Connecting
  //  connect(_eventsTable, &QTableWidget::cellDoubleClicked, this,
  //          &WorkPage::handleEventClicked);
  connect(_eventsTable, &TableAssistant::viewClicked,
          [this](auto uuid) { emit viewEventClicked(uuid); });
  connect(_eventsTable, &TableAssistant::removeClicked,
          [this](auto uuid) { emit removeEventClicked(uuid); });
  // Connecting end

  // Layout`s
  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->addWidget(_infoProject);
  vLayout->addWidget(container, 1);
  //  vLayout->addStretch(1);
  vLayout->addWidget(_eventsTable);

  setLayout(vLayout);
  // Layout`s end
}

void WorkPage::loadProject(const std::unique_ptr<Data::SeismProject> &project) {
  _infoProject->update(project);
  _surface->setProject(project);
  _eventsTable->setAll<SeismEvent>(project->getAllMap<SeismEvent>());
}

void WorkPage::updateProject(const std::unique_ptr<Data::SeismEvent> &event) {
  _infoProject->addEvent();
  _surface->addEvent(event);

  _eventsTable->add<SeismEvent>(event);
}

void WorkPage::updateProject(
    const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &events) {
  _eventsTable->setAll<SeismEvent>(events);

  for (auto &itr : events) {
    _surface->showEvent(itr.first);
  }
}

void WorkPage::updateProjectRemoveEvent(const QUuid &uuid) {
  _infoProject->removeEvent();
  _surface->removeEvent(uuid);
  _eventsTable->remove<SeismEvent>(uuid);
}

void WorkPage::updateProject(
    const std::unique_ptr<Data::SeismHorizon> &horizon) {
  _infoProject->addHorizon();
  _surface->addHorizon(horizon);
}

void WorkPage::updateProjectRemoveHorizon(const QUuid &uuid) {
  _infoProject->removeHorizon();
  _surface->removeHorizon(uuid);
}

void WorkPage::updateProject(const std::unique_ptr<Data::SeismWell> &well) {
  _infoProject->addWell();
  _surface->addWell(well);
  for (auto &receiver : well->getReceivers()) {
    _surface->addReceiver(receiver);
  }
}

void WorkPage::updateProjectRemoveWell(const QUuid &uuid) {
  _infoProject->removeWell();
  _surface->removeWell(uuid);
}

// void WorkPage::handleEventClicked(int row, int col) {
//  QUuid uuid = _eventsTable->item(row, 0)->text();

//  if (6 == col) {
//    emit removeEventClicked(uuid);
//    return;
//  }

//  emit viewEventClicked(uuid);
//}

// void WorkPage::clearTable() {
//  int end = _eventsTable->rowCount();
//  for (int i = 0; i < end; ++i) {
//    _eventsTable->removeRow(0);
//  }
//}

// void WorkPage::initEventsTable(QTableWidget *table) {
//  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
//  table->setColumnCount(8);

//  table->setSortingEnabled(true);

//  // configure column settings
//  //  table->setHorizontalHeaderLabels(
//  //      QStringList() << "uuid"
//  //                    << "Component Number"
//  //                    << "Type"
//  //                    << "Location"
//  //                    << "Date"
//  //                    << "Time"
//  //                    << "Remove"
//  //                    <<
//  "TestTestTestTestTestTestTestTestTestTestTestTest");

//  QTableWidgetItem *item = new QTableWidgetItem("Name");
//  table->setHorizontalHeaderItem(1, item);
//  table->setCellWidget(0, 1, new QPushButton("button"));

//  table->setColumnHidden(0, true);

//  table->setSelectionBehavior(QAbstractItemView::SelectRows);
//  table->setSelectionMode(QAbstractItemView::SingleSelection);

//  table->setContentsMargins(100, 10, 10, 10);

//  _eventsTable->resizeColumnsToContents();
//}

// void WorkPage::insertEventInTable(
//    const std::unique_ptr<Data::SeismEvent> &event) {

//  _eventsTable->insertRow(_eventsTable->rowCount());

//  int insertRow = _eventsTable->rowCount() - 1;

//  _eventsTable->setItem(insertRow, 0,
//                        new QTableWidgetItem(event->getUuid().toString()));

//  _eventsTable->setItem(
//      insertRow, 1,
//      new QTableWidgetItem(QString::number(event->getComponentNumber())));

//  _eventsTable->setItem(
//      insertRow, 2, new QTableWidgetItem(QString::number(event->getType())));

//  if (event->isProcessed()) {
//    _eventsTable->setItem(
//        insertRow, 3,
//        new QTableWidgetItem(
//            QString("%1 %2 %3")
//                .arg(static_cast<double>(std::get<0>(event->getLocation())),
//                0,
//                     'f', 2)
//                .arg(static_cast<double>(std::get<1>(event->getLocation())),
//                0,
//                     'f', 2)
//                .arg(static_cast<double>(std::get<2>(event->getLocation())),
//                0,
//                     'f', 2)));
//  }

//  _eventsTable->setItem(
//      insertRow, 4,
//      new QTableWidgetItem(event->getDateTime().date().toString("dd.MM.yy")));

//  _eventsTable->setItem(
//      insertRow, 5,
//      new QTableWidgetItem(event->getDateTime().time().toString("hh:mm")));

//  QTableWidgetItem *removeItem = new QTableWidgetItem("Remove");
//  removeItem->setTextAlignment(Qt::AlignCenter);
//  removeItem->setBackground(Qt::red);
//  _eventsTable->setItem(insertRow, 6, removeItem);

//  _eventsTable->resizeRowsToContents();
//  _eventsTable->resizeColumnsToContents();
//}

// void WorkPage::removeEventInTable(const QUuid &uuid) {
//  const QString str_uuid = uuid.toString();
//  for (int row = 0; row < _eventsTable->rowCount(); row++) {
//    if (str_uuid == _eventsTable->item(row, 0)->text()) {
//      _eventsTable->removeRow(row);
//      return;
//    }
//  }
//}

} // namespace Main
