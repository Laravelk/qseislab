#include "controller.h"

namespace EventOperation {
namespace ViewEvent {
Controller::Controller(QObject *parent)
    : QObject(parent), _view(std::make_unique<View>()) {

  connect(_view.get(), &View::finished, [this] { emit finished(); });
}

void Controller::viewEvent(
    const std::unique_ptr<Data::SeismEvent> &event) const {
  _view->update(event);
  _view->setModal(true);
  _view->show();
}

// void Controller::finish(int /*result*/) { emit finished(); }

} // namespace ViewEvent
} // namespace EventOperation
