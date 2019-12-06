#include "workpage.h"

//#include "../oil_field_scene/oilFieldScene.h"
#include "data/seismhorizon.h"
#include "data/seismproject.h"
#include "data/seismreceiver.h"
#include "infoproject.h"
#include "main/oil_field_scene/oilfieldwidget.h"

#include "share_view/filteringtableassistant.h"

#include <iostream> // TODO: delete

#include <QDateTime>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>

//#include "event_operation/view_event/view.h" // TODO: не должно быть этого!!

typedef Data::SeismProject SeismProject;
typedef Data::SeismEvent SeismEvent;
typedef Data::SeismHorizon SeismHorizon;
typedef Data::SeismReceiver SeismReceiver;

namespace Main {
WorkPage::WorkPage(QWidget *parent)
    : QFrame(parent), _workPages(new QTabWidget()),
      _oilFieldWidget(new OilFieldWidget()),
      _eventsTable(
          new FilteringTableAssistant(FilteringTableAssistant::ForEvents)) {

  // Setting`s

  _workPages->setTabsClosable(true);
  _workPages->setTabBarAutoHide(true);
  connect(_workPages, &QTabWidget::tabCloseRequested, [this](auto index) {
    if (0 != index) {
      auto page = _workPages->widget(index);
      // TODO: очень плохо - не надо так!
      //      if (std::is_same<EventOperation::ViewEvent::View *,
      //                       decltype(page)>::value) {
      if (static_cast<EventOperation::ViewEvent::View *>(page)->allValid()) {
        _workPages->removeTab(index);
        emit eventPageClosed(_pages_uuids_map.at(page));
      }
      //      }
    }
  });

  _workPages->addTab(_oilFieldWidget, "Oil Field");
  _pages_uuids_map[_oilFieldWidget] = QUuid();
  _workPages->setTabIcon(0, QIcon());
  // Setting`s end

  // Connecting
  //  connect(_workPages, &QTabWidget::currentChanged, [this](auto index) {
  //    emit eventPageChanged(_pages_uuids_map[_workPages->widget(index)]);
  //  });

  connect(_eventsTable, &FilteringTableAssistant::captureFocus,
          [this] { emit eventPageChanged(_pages_uuids_map[_oilFieldWidget]); });
  //  connect(_eventsTable, &FilteringTableAssistant::freeFocus, [this] {
  //    emit eventPageChanged((_pages_uuids_map[_workPages->currentWidget()]));
  //  });

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

void WorkPage::addEventPage(QWidget *page, SeismEvent const *const event) {
  _pages_uuids_map[page] = event->getUuid();
  _workPages->addTab(page, event->getName());
  connect(event, &SeismEvent::infoChanged, [this, page](auto event) {
    auto index = _workPages->indexOf(page);
    auto &name = event->getName();
    _workPages->setTabText(index, name);
  });
  _workPages->setCurrentWidget(page);

  // TODO: очень плохо - не надо так!
  auto eventPage = static_cast<EventOperation::ViewEvent::View *>(page);
  connect(eventPage, &EventOperation::ViewEvent::View::captureFocus,
          [this, event] { emit eventPageChanged(event->getUuid()); });
}

void WorkPage::setFocusEventPage(QWidget *page) {
  _workPages->setCurrentWidget(page);
}

// QUuid WorkPage::getFocusEventPage() const {
//  if (_workPages->hasFocus()) {
//    int index = _workPages->currentIndex();
//    if (0 != index) {
//      auto page = _workPages->widget(index);
//      return _pages_uuids_map.at(page);
//    }
//  }

//  return QUuid();
//}

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
  //  for (auto &receiver : well->getReceivers()) {
  //    //    _oilFieldScene->addReceiver(receiver);
  //    _oilFieldWidget->addReceiver(receiver.get());
  //  }
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
