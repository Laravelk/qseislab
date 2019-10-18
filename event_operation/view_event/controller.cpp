#include "controller.h"

#include "data/seismevent.h"

#include <iostream> // TODO: remove

typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace ViewEvent {
Controller::Controller(QObject *parent)
    : QObject(parent), _view(std::make_unique<View>()) {

  connect(_view.get(), &View::sendPicksInfo,
          [this](const auto type, const auto num, const auto l_val,
                 const auto pick_val, const auto r_val) {
            int idx = 0;
            for (auto &component : _event->getComponents()) {
              if (num == idx) {
                Data::SeismWavePick wavePick =
                    Data::SeismWavePick(type, pick_val);
                wavePick.setPolarizationLeftBorder(l_val);
                wavePick.setPolarizationRightBorder(r_val);

                component->addWavePick(wavePick);
                break;
              }
              ++idx;
            }
          });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::viewEvent(const std::unique_ptr<SeismEvent> &event) {
  _event = std::make_unique<SeismEvent>(*event);
  connect(_event.get(), &Data::SeismEvent::changed,
          []() { std::cout << "event changed" << std::endl; });

  _view->update(_event);
  _view->setModal(true);
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    _view->settingEventInfo(_event);
    emit sendEvent(_event);
  }

  emit finished();
}

} // namespace ViewEvent
} // namespace EventOperation
