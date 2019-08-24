#include "controller.h"

#include "data/seismproject.h"
#include "data/seismreciever.h"

typedef Data::SeismReciever SeismReciever;
typedef Data::SeismProject SeismProject;

namespace RecieverOperation {
Controller::Controller(QObject *parent)
    : QObject(parent), _view(std::make_unique<View>()) {

  connect(_view.get(), SIGNAL(addRecieverClicked()), this,
          SLOT(handleAddRecieverClicked()));
  connect(_view.get(), SIGNAL(removeRecieverClicked(const QUuid)), this,
          SLOT(handleRemoveRecieverClicked(const QUuid)));
  connect(_view.get(), SIGNAL(finished(int)), this, SLOT(finish(int)));
}

void Controller::viewRecievers(const std::unique_ptr<SeismProject> &project) {
  for (auto &reciever : project->getAllList<SeismReciever>()) {
    _view->addReciever(reciever);
  }

  _view->setModal(true);
  _view->show();
}

void Controller::handleRemoveRecieverClicked(const QUuid uuid) {
  _view->removeReciever(uuid);
  _view->changed(true);
  _removedRecievers.push_back(uuid);
}

void Controller::handleAddRecieverClicked() {
  std::unique_ptr<SeismReciever> newReciever =
      std::make_unique<SeismReciever>();
  _view->settingRecieverInfo(newReciever);
  _view->addReciever(newReciever);
  _view->changed(true);
  _newRecievers.push_back(std::move(newReciever));
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    for (auto &removedReciever : _removedRecievers) {
      emit sendRemovedReciever(removedReciever);
    }
    for (auto &reciever : _newRecievers) {
      emit sendReciever(reciever);
    }
  }

  emit finished();
}

} // namespace RecieverOperation
