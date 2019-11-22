#include "view.h"

#include "event_operation/share/view/graphiccontroller.h"
#include "event_operation/share/view/infoevent.h"

#include <assert.h>

//#include "data/seismevent.h" // TODO: delete
// #include <iostream> // TODO: delete

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace ViewEvent {
View::View(const std::set<QString> &eventNames, SeismEvent const *const event,
           QWidget *parent)
    : QWidget(parent), _infoEvent(new InfoEvent()),
      _graphicEvent(new GraphicController()), _eventNames(eventNames) {

  // Setting`s
  setWindowTitle("SeismWindow");
  setMinimumSize(1300, 590);
  // Setting`s end

  // Connecting
  connect(_infoEvent, &InfoEvent::nameChanged, [this](auto &name) {
    updateRepetition(name);
    _graphicEvent->updateEventName(name);
  });
  connect(_graphicEvent, &EventOperation::GraphicController::sendPicksInfo,
          [this](auto type, auto num, auto l_val, auto pick_val, auto r_val) {
            emit sendPicksInfo(type, num, l_val, pick_val, r_val);
          });
  connect(_graphicEvent,
          &EventOperation::GraphicController::
              createPolarizationAnalysisWindowClicked,
          [this]() { emit createPolarizationAnalysisWindow(); });
  // Connecting end

  update(event);

  // Layout`s
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(_infoEvent);
  leftLayout->addStretch(1);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  mainLayout->addWidget(_graphicEvent->getView(), 10);

  QVBoxLayout *mainButtonLayout = new QVBoxLayout();
  mainButtonLayout->addLayout(mainLayout);
  mainButtonLayout->addStretch(1);

  setLayout(mainButtonLayout);
  // Layout`s end
}

void View::update(SeismEvent const *const event) {
  _infoEvent->update(event);
  _graphicEvent->update(event);
}

void View::settingEventInfo(SeismEvent *const event) const {
  _infoEvent->settingEventInfo(event);
}

ChartGesture *View::getChartGesture() { return _graphicEvent->getModel(); }

void View::finish() const { emit finished(); }

void View::updateRepetition(const QString &name) {
  for (auto &globalName : _eventNames) {
    if (name == globalName) {
      _isValid = false;
      _infoEvent->setBrush(Qt::red);
      return;
    }
  }
  _isValid = true;
  _infoEvent->setBrush(Qt::white);
}

} // namespace ViewEvent
} // namespace EventOperation
