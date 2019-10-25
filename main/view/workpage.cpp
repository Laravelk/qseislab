#include "workpage.h"

#include "../oil_field_scene/oilFieldScene.h"
#include "data/seismhorizon.h"
#include "data/seismproject.h"
#include "data/seismreceiver.h"
#include "infoproject.h"

#include "share_view/filteringtableassistant.h"

//#include <iostream> // TODO: delete

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
    : QFrame(parent), _eventsTable(new FilteringTableAssistant(
                          FilteringTableAssistant::ForEvents)),
      _graph(new Q3DSurface), _eventBox(new QCheckBox),
      _receiverBox(new QCheckBox), _wellBox(new QCheckBox),
      _horizonBox(new QCheckBox) {

  // Setting`s
  _graph->setMinimumWidth(400);
  _graph->setMinimumHeight(700);
  _oilFieldScene = new OilFieldScene(_graph);
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
  connect(_eventsTable, &FilteringTableAssistant::viewClicked,
          [this](auto &uuid) { emit viewEventClicked(uuid); });
  connect(_eventsTable, &FilteringTableAssistant::removeClicked,
          [this](auto &uuid) { emit removeEventClicked(uuid); });

  // NOTE: link filter with graph
  connect(_eventsTable, &FilteringTableAssistant::hide, _oilFieldScene,
          &Main::OilFieldScene::hideEvent);
  connect(_eventsTable, &FilteringTableAssistant::show, _oilFieldScene,
          &Main::OilFieldScene::showEvent);
  // end link

  connect(_horizonBox, &QCheckBox::stateChanged, [this]() {
    _oilFieldScene->hideAllHorizon(!_oilFieldScene->isHorizonsHide());
  });
  connect(_receiverBox, &QCheckBox::stateChanged, [this]() {
    _oilFieldScene->hideAllReceiver(!_oilFieldScene->isReceiversHide());
  });
  connect(_wellBox, &QCheckBox::stateChanged, [this]() {
    _oilFieldScene->hideAllWell(!_oilFieldScene->isWellsHide());
  });
  connect(_eventBox, &QCheckBox::stateChanged, [this]() {
    _oilFieldScene->hideAllEvent(!_oilFieldScene->isEventsHide());
  });
  // Connecting end

  // Layout`s
  QHBoxLayout *oilFieldSceneLayout = new QHBoxLayout();
  oilFieldSceneLayout->addWidget(container, 1);
  QVBoxLayout *checkLayout = new QVBoxLayout();
  checkLayout->addStretch(1);
  checkLayout->addWidget(_horizonBox);
  checkLayout->addWidget(_receiverBox);
  checkLayout->addWidget(_wellBox);
  checkLayout->addWidget(_eventBox);
  checkLayout->addStretch(1);
  oilFieldSceneLayout->addLayout(checkLayout);
  QVBoxLayout *vLayout = new QVBoxLayout();

  //  vLayout->addLayout(checkLayout);
  //  vLayout->addWidget(container, 1);
  vLayout->addLayout(oilFieldSceneLayout);
  //  vLayout->addStretch(1);
  vLayout->addWidget(_eventsTable);

  setLayout(vLayout);
  // Layout`s end
}

void WorkPage::loadProject(const std::unique_ptr<Data::SeismProject> &project) {
  _oilFieldScene->setProject(project);
  _eventsTable->setAll<SeismEvent>(project->getAllMap<SeismEvent>());
}

void WorkPage::addEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  _oilFieldScene->addEvent(event); // TODO: uncoment
  _eventsTable->add<SeismEvent>(event);
}

void WorkPage::processedEvents(
    const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &events) {
  _eventsTable->setAll<SeismEvent>(events);

  for (auto &itr : events) {
    _oilFieldScene->showEvent(itr.first); // TODO: uncomment
  }
}

void WorkPage::updateEvent(const std::unique_ptr<Data::SeismEvent> &event) {
  _eventsTable->update<SeismEvent>(event);

  //  _oilFieldScene->updateEvent(event); // TODO: uncomment and realize!
}

void WorkPage::removeEvent(const QUuid &uuid) {
  _oilFieldScene->removeEvent(uuid); // TODO: uncomment
  _eventsTable->remove(uuid);
}

void WorkPage::addHorizon(const std::unique_ptr<Data::SeismHorizon> &horizon) {
  _oilFieldScene->addHorizon(horizon);
}

void WorkPage::removeHorizon(const QUuid &uuid) {
  _oilFieldScene->removeHorizon(uuid);
}

void WorkPage::addWell(const std::unique_ptr<Data::SeismWell> &well) {
  _oilFieldScene->addWell(well);
  for (auto &receiver : well->getReceivers()) {
    _oilFieldScene->addReceiver(receiver);
  }
}

void WorkPage::removeWell(const QUuid &uuid) {
  _oilFieldScene->removeWell(uuid);
}

void WorkPage::addReceiver(
    const std::unique_ptr<Data::SeismReceiver> &receiver) {
  _oilFieldScene->addReceiver(receiver);
}

void WorkPage::removeReceiver(const QUuid &uuid) {
  _oilFieldScene->removeReceiver(uuid);
}

} // namespace Main
