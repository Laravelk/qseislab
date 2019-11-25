#include "workpage.h"

//#include "../oil_field_scene/oilFieldScene.h"
#include "data/seismhorizon.h"
#include "data/seismproject.h"
#include "data/seismreceiver.h"
#include "infoproject.h"
#include "main/oil_field_scene/oilfieldwidget.h"

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
    : QFrame(parent), _workPages(new QTabWidget()),
      _oilFieldWidget(new OilFieldWidget()),
      _eventsTable(
          new FilteringTableAssistant(FilteringTableAssistant::ForEvents))
//      ,_graph(new Q3DSurface), _eventBox(new QCheckBox),
//      _receiverBox(new QCheckBox), _wellBox(new QCheckBox),
//      _horizonBox(new QCheckBox)
{

  // Setting`s

  _workPages->setTabsClosable(true);
  _workPages->setTabBarAutoHide(true);
  //  connect(_workPages, &QTabWidget::tabCloseRequested, _workPages,
  //          &QTabWidget::removeTab);
  connect(_workPages, &QTabWidget::tabCloseRequested, [this](auto index) {
    if (0 != index) {
      auto page = _workPages->widget(index);
      _workPages->removeTab(index);
      emit eventPageClosed(_pages_uuids_map.at(page));
    }
  });

  _workPages->addTab(_oilFieldWidget, "Oil Field");
  _workPages->setTabIcon(0, QIcon());

  //  _graph->setMinimumWidth(400);
  //  _graph->setMinimumHeight(700);
  //  _oilFieldScene = new OilFieldScene(_graph);
  //  QWidget *container = QWidget::createWindowContainer(_graph);
  //  container->setMinimumSize(QSize(400, 400));
  //  container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //  container->setFocusPolicy(Qt::StrongFocus);

  //  _horizonBox->setChecked(true);
  //  _horizonBox->setText("Horizons");
  //  _wellBox->setChecked(true);
  //  _wellBox->setText("Wells");
  //  _receiverBox->setChecked(true);
  //  _receiverBox->setText("Receivers");
  //  _eventBox->setChecked(true);
  //  _eventBox->setText("Events");

  // Setting`s end

  // Connecting
  connect(_workPages, &QTabWidget::currentChanged, [this](auto index) {
    if (0 == index) {
      emit eventSelectionChanged(_eventsTable->objectSelection());
    } else {
      emit eventPageChanged(_pages_uuids_map[_workPages->widget(index)]);
    }
  });

  connect(_eventsTable, &FilteringTableAssistant::objectSelectionChanged,
          [this](auto &select) { emit eventSelectionChanged(select); });

  connect(_eventsTable, &FilteringTableAssistant::viewClicked,
          [this](auto &uuid) { emit viewEventClicked(uuid); });
  connect(_eventsTable, &FilteringTableAssistant::removeClicked,
          [this](auto &uuid) { emit removeEventClicked(uuid); });

  // NOTE: link filter with graph
  //  connect(_eventsTable, &FilteringTableAssistant::hide, _oilFieldScene,
  //          &Main::OilFieldScene::hideEvent);
  //  connect(_eventsTable, &FilteringTableAssistant::show, _oilFieldScene,
  //          &Main::OilFieldScene::showEvent);
  connect(_eventsTable, &FilteringTableAssistant::hide, _oilFieldWidget,
          &Main::OilFieldWidget::hideSeismEvent);
  connect(_eventsTable, &FilteringTableAssistant::show, _oilFieldWidget,
          &Main::OilFieldWidget::showSeismEvent);
  // end link

  //  connect(_horizonBox, &QCheckBox::stateChanged, [this]() {
  //    _oilFieldScene->hideAllHorizon(!_oilFieldScene->isHorizonsHide());
  //  });
  //  connect(_receiverBox, &QCheckBox::stateChanged, [this]() {
  //    _oilFieldScene->hideAllReceiver(!_oilFieldScene->isReceiversHide());
  //  });
  //  connect(_wellBox, &QCheckBox::stateChanged, [this]() {
  //    _oilFieldScene->hideAllWell(!_oilFieldScene->isWellsHide());
  //  });
  //  connect(_eventBox, &QCheckBox::stateChanged, [this]() {
  //    _oilFieldScene->hideAllEvent(!_oilFieldScene->isEventsHide());
  //  });
  //  connect(_wellBox, &QCheckBox::stateChanged,
  //          [this]() { _surface->hideAllWell(!_surface->isWellsHide()); });
  //  connect(_eventBox, &QCheckBox::stateChanged,
  //          [this]() { _surface->hideAllEvent(!_surface->isEventsHide()); });
  // Connecting end

  // Layout`s
  //  QHBoxLayout *oilFieldSceneLayout = new QHBoxLayout();
  //  oilFieldSceneLayout->addWidget(container, 1);
  //  QVBoxLayout *checkLayout = new QVBoxLayout();
  //  checkLayout->addStretch(1);
  //  checkLayout->addWidget(_horizonBox);
  //  checkLayout->addWidget(_receiverBox);
  //  checkLayout->addWidget(_wellBox);
  //  checkLayout->addWidget(_eventBox);
  //  checkLayout->addStretch(1);
  //  oilFieldSceneLayout->addLayout(checkLayout);
  //  QWidget *oilFieldSceneWidget = new QWidget();
  //  oilFieldSceneWidget->setLayout(oilFieldSceneLayout);

  QSplitter *splitter = new QSplitter(Qt::Vertical);
  splitter->setChildrenCollapsible(false);
  //  splitter->addWidget(oilFieldSceneWidget);
  splitter->addWidget(_workPages);
  splitter->addWidget(_eventsTable);

  QVBoxLayout *vLayout = new QVBoxLayout();
  //  vLayout->addLayout(oilFieldSceneLayout);
  //  vLayout->addWidget(_eventsTable);
  vLayout->addWidget(splitter);

  setLayout(vLayout);
  // Layout`s end
}

void WorkPage::loadProject(Data::SeismProject const *const project) {
  //  _surface->setProject(project);
  //  _oilFieldScene->setProject(project);
  _oilFieldWidget->setProject(project);
  //  _eventsTable->setAll<SeismEvent>(project->getAllMap<SeismEvent>());
  _eventsTable->setAll<SeismEvent>(project->getAllMap<SeismEvent>());
}

void WorkPage::addEventPage(QWidget *eventPage, SeismEvent const *const event) {
  _pages_uuids_map[eventPage] = event->getUuid();
  _workPages->addTab(eventPage, event->getInfo()->getName());
  _workPages->setCurrentWidget(eventPage);
}

void WorkPage::setFocusEventPage(QWidget *page) {
  _workPages->setCurrentWidget(page);
}

// const QUuid WorkPage::getFocusEvent() const {
//  return _eventsTable->getFocusObject();
//}

void WorkPage::addEvent(Data::SeismEvent const *const event) {
  //  _oilFieldScene->addEvent(event);
  _oilFieldWidget->addEvent(event);
  _eventsTable->add<SeismEvent>(event);
}

void WorkPage::processedEvents(
    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &events) {
  _eventsTable->setAll<SeismEvent>(events);

  for (auto &itr : events) {
    //    _oilFieldScene->showEvent(itr.first);
    _oilFieldWidget->showSeismEvent(itr.first);
  }
}

void WorkPage::updateEvent(Data::SeismEvent const *const event) {
  _eventsTable->update<SeismEvent>(event);

  //  _oilFieldScene->updateEvent(event); // TODO: uncomment and realize!
}

void WorkPage::removeEvent(const QUuid &uuid) {
  //  _oilFieldScene->removeEvent(uuid);
  _oilFieldWidget->removeEvent(uuid);
  _eventsTable->remove(uuid);
}

void WorkPage::addHorizon(Data::SeismHorizon const *const horizon) {
  //  _surface->addHorizon(horizon);
  //  _oilFieldScene->addHorizon(horizon);
  _oilFieldWidget->addHorizon(horizon);
}

void WorkPage::removeHorizon(const QUuid &uuid) {
  //  _surface->removeHorizon(uuid);
  //  _oilFieldScene->removeHorizon(uuid);
  _oilFieldWidget->removeHorizon(uuid);
}

void WorkPage::addWell(Data::SeismWell const *const well) {
  //  _surface->addWell(well);
  //  _oilFieldScene->addWell(well);
  _oilFieldWidget->addWell(well);
  for (auto &receiver : well->getReceivers()) {
    //    _oilFieldScene->addReceiver(receiver);
    _oilFieldWidget->addReceiver(receiver.get());
  }
}

void WorkPage::removeWell(const QUuid &uuid) {
  //    _surface->removeWell(uuid);
  //  _oilFieldScene->removeWell(uuid);
  _oilFieldWidget->removeWell(uuid);
}

void WorkPage::addReceiver(Data::SeismReceiver const *const receiver) {
  //  _surface->addReceiver(receiver);
  //  _oilFieldScene->addReceiver(receiver);
  _oilFieldWidget->addReceiver(receiver);
}

void WorkPage::removeReceiver(const QUuid &uuid) {
  //  _surface->removeReceiver(uuid);
  //  _oilFieldScene->removeReceiver(uuid);
  _oilFieldWidget->removeReceiver(uuid);
}

} // namespace Main
