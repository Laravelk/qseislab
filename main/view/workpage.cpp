#include "workpage.h"

#include "../surface_view/surface.h"
#include "data/seismhorizon.h"
#include "data/seismproject.h"
#include "data/seismreceiver.h"
#include "infoproject.h"

#include "share_view/tableassistant.h"

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
      _graph(new Q3DSurface), _eventBox(new QCheckBox),
      _receiverBox(new QCheckBox), _wellBox(new QCheckBox),
      _horizonBox(new QCheckBox) {

  // Setting`s
  //  initEventsTable(_eventsTable);
  _graph->setMinimumWidth(400);
  _graph->setMinimumHeight(700);
  _surface = new Surface(_graph);
  QWidget *container = QWidget::createWindowContainer(_graph);
  container->setMinimumSize(QSize(400, 400));
  container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  container->setFocusPolicy(Qt::StrongFocus);

  _horizonBox->setChecked(true);
  _horizonBox->setText("Horizons");
  _wellBox->setChecked(true);
  _wellBox->setText("Wells");
  _receiverBox->setChecked(true);
  _receiverBox->setText("Receivers");
  _eventBox->setChecked(true);
  _eventBox->setText("Events");

  // Setting`s end

  // Connecting
  //  connect(_eventsTable, &QTableWidget::cellDoubleClicked, this,
  //          &WorkPage::handleEventClicked);
  connect(_eventsTable, &TableAssistant::viewClicked,
          [this](auto uuid) { emit viewEventClicked(uuid); });
  connect(_eventsTable, &TableAssistant::removeClicked,
          [this](auto uuid) { emit removeEventClicked(uuid); });
  connect(_horizonBox, &QCheckBox::stateChanged,
          [this]() { _surface->hideAllHorizon(!_surface->isHorizonsHide()); });
  connect(_receiverBox, &QCheckBox::stateChanged, [this]() {
    _surface->hideAllReceiver(!_surface->isReceiversHide());
  });
  connect(_wellBox, &QCheckBox::stateChanged,
          [this]() { _surface->hideAllWell(!_surface->isWellsHide()); });
  connect(_eventBox, &QCheckBox::stateChanged,
          [this]() { _surface->hideAllEvent(!_surface->isEventsHide()); });
  // Connecting end

  // Layout`s
  QHBoxLayout *checkLayout = new QHBoxLayout();
  checkLayout->addWidget(_horizonBox);
  checkLayout->addWidget(_receiverBox);
  checkLayout->addWidget(_wellBox);
  checkLayout->addWidget(_eventBox);
  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->addLayout(checkLayout);
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

void WorkPage::addEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  _infoProject->addEvent();

  //  _surface->addEvent(event); // TODO: uncoment

  _eventsTable->add<SeismEvent>(event);
}

void WorkPage::processedEvents(
    const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &events) {
  _eventsTable->setAll<SeismEvent>(events);

  for (auto &itr : events) {
    //    _surface->showEvent(itr.first); // TODO: uncomment
  }
}

void WorkPage::updateEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  //    _infoProject->updateEvent(event); // NOTE: нужно ли? (не реализовано)
  _eventsTable->update<SeismEvent>(event);

  //    _surface->updateEvent(event); // TODO: uncomment and realize!
}

void WorkPage::removeEvent(const QUuid &uuid) {
  _infoProject->removeEvent();
  //  _surface->removeEvent(uuid); // TODO: uncomment
  _eventsTable->remove<SeismEvent>(uuid);
}

void WorkPage::addHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon) {
  _infoProject->addHorizon();
  _surface->addHorizon(horizon);
}

void WorkPage::removeHorizon(const QUuid &uuid) {
  _infoProject->removeHorizon();
  _surface->removeHorizon(uuid);
}

void WorkPage::addWell(const std::unique_ptr<Data::SeismWell> &well) {
  _infoProject->addWell();
  _surface->addWell(well);
  for (auto &receiver : well->getReceivers()) {
    _surface->addReceiver(receiver);
  }
}

void WorkPage::removeWell(const QUuid &uuid) {
  _infoProject->removeWell();
  _surface->removeWell(uuid);
}

} // namespace Main
