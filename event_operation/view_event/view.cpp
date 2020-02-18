#include "view.h"

#include "event_operation/share/view/eventtoolswidget.h"
#include "event_operation/share/view/graphiccontroller.h"
#include "event_operation/share/view/polar_graph/polargraph.h"
#include "event_operation/share/view/infoevent.h"

#include <assert.h>

#include <iostream> // TODO: delete

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace ViewEvent {
View::View(const std::set<QString> &eventNames, SeismEvent const *const event,
           QWidget *parent)
    : QWidget(parent), _toolsWidget(new EventToolsWidget()),
      _infoEvent(new InfoEvent()), _graphicEvent(new GraphicController()),
      _eventNames(eventNames) {

  // Setting`s
  setFocusPolicy(Qt::StrongFocus);

  setWindowTitle("SeismWindow");
  setMinimumSize(1300, 590);
  // Setting`s end

  // Connecting
  connect(_toolsWidget, &EventToolsWidget::undoClicked,
          [this] { emit undoClicked(); });
  connect(_toolsWidget, &EventToolsWidget::redoClicked,
          [this] { emit redoClicked(); });
  connect(_toolsWidget, &EventToolsWidget::eventTransformClicked,
          [this](auto oper) { emit eventActionClicked(oper); });

  connect(_infoEvent, &InfoEvent::changed, [this]() { emit infoChanged(); });

  connect(_graphicEvent, &EventOperation::GraphicController::sendPicksInfo,
          [this](auto type, auto num, auto l_val, auto pick_val, auto r_val) {
            emit sendPicksInfo(type, num, l_val, pick_val, r_val);
          });
  connect(_graphicEvent, &EventOperation::GraphicController::removePick,
          [this](auto type, auto num) { emit removePick(type, num); });
  connect(_graphicEvent, &EventOperation::GraphicController::addPick,
          [this](auto type, auto num, auto l_val, auto arrival, auto r_val) {
            emit addPick(type, num, l_val, arrival, r_val);
          });
  connect(_graphicEvent,
          &EventOperation::GraphicController::
              calculatePolarizationAnalysisDataClicked,
          [this]() { emit calculatePolarizationAnalysisData(); });

  connect(_graphicEvent,
          &EventOperation::GraphicController::
              createPolarizationAnalysisWindowClicked,
          [this]() { emit createPolarizationAnalysisWindow(); });
  // Connecting end

  updateInfoEvent(event);
  updateDataEvent(event);

  // Layout`s
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(_infoEvent);
  leftLayout->addStretch(1);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  mainLayout->addWidget(_graphicEvent, 10);

  QVBoxLayout *mainButtonLayout = new QVBoxLayout();
  mainButtonLayout->addWidget(_toolsWidget);
  mainButtonLayout->addLayout(mainLayout);
  mainButtonLayout->addStretch(1);

  setLayout(mainButtonLayout);
  // Layout`s end
}

bool View::allValid() const { return _isValid; }

void View::updateInfoEvent(Data::SeismEvent const *const event) {
  auto &name = event->getName();
  updateRepetition(name);
  _infoEvent->update(event);
  _graphicEvent->updateEventName(name);
}

void View::updateDataEvent(Data::SeismEvent const *const event) {
  _infoEvent->update(event);
  _graphicEvent->update(event);
}

void View::settingEventInfo(SeismEvent *const event) const {
  _infoEvent->settingEventInfo(event);
}

void View::updatePolarGraph(const Data::SeismEvent *const event) {
  _graphicEvent->updatePolarGraph(event);
}

ChartGesture *View::getChartGesture() { return _graphicEvent->getModel(); }

void View::updateRepetition(const QString &name) {
  for (auto &globalName : _eventNames) {
    if (name == globalName) {
      _isValid = false;
      _infoEvent->setBrush(Qt::red);
      return;
    }
  }
  _isValid = true;
  _infoEvent->setBrush(Qt::transparent);
}

} // namespace ViewEvent
} // namespace EventOperation
