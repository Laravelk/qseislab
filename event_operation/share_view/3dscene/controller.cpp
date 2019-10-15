#include "controller.h"
#include "model.h"

namespace EventOperation {
namespace VolumetricScene {

Controller::Controller(QObject *parent)
    : QObject(parent), _model(new Model(this)) {
  _view = std::make_unique<View>();
}

void Controller::start() { _view->show(); }

} // namespace VolumetricScene
} // namespace EventOperation
