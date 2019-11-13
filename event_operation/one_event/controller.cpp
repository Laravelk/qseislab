#include "controller.h"

#include "data/seismevent.h"
#include "data/seismwell.h"
#include "event_operation/share/model.h"
#include "event_operation/share/view/3dscene/polarizationanalysiswindow.h"

#include "event_operation/modification/rotatedatatoebasis.h"
#include "event_operation/modification/testmultiplier.h"

#include "data/io/segyreader.h"

#include <iostream> // TODO: remove

typedef Data::IO::SegyReader SegyReader;
typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace OneEvent {
Controller::Controller(
    const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &all_events,
    const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &wells_map,
    QObject *parent)
    : QObject(parent), _model(new Model(new SegyReader(), this)),
      _event(std::make_unique<Data::SeismEvent>()),
      //      _appliedOperations(new QUndoStack()),
      _undoStack(std::make_unique<QUndoStack>()) {

  // prepare data for view
  std::map<QUuid, QString> wellNames_map;
  for (auto &uuid_well : wells_map) {
    wellNames_map[uuid_well.first] = uuid_well.second->getName();
  }
  std::set<QString> eventNames;
  for (auto &uuid_event : all_events) {
    eventNames.insert(uuid_event.second->getName());
  }
  _view = std::make_unique<View>(eventNames, wellNames_map, _undoStack);
  // ...

  connect(_event.get(), &Data::SeismEvent::changed, []() {
    //              std::cout << "event changed" << std::endl;
  });

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::sendWellUuidAndFilePath,
          [this, &wells_map](auto &wellUuid, auto &filePath) {
            auto components =
                _model->getSeismComponents(wells_map.at(wellUuid), filePath);
            if (!components.empty()) {
              for (auto &component : components) {
                _event->addComponent(std::move(component));
              }
              _eventNameContainer[wellUuid] = QFileInfo(filePath).baseName();
              _event->setName(generateEventName());
              _view->update(_event, wellUuid);
            }
          });
  connect(_view.get(), &View::createPolarizationAnalysisWindow, [this]() {
    _polarizationWindow = new PolarizationAnalysisWindow(_event);
    _polarizationWindow->show();
  });

  connect(_view.get(), &View::sendWellUuidForRemove,
          [this, &wells_map](auto &uuid) {
            auto &well = wells_map.at(uuid);
            for (auto &reciever : well->getReceivers()) {
              _event->removeComponentByReceiverUuid(reciever->getUuid());
            }
            _eventNameContainer.erase(uuid);
            _event->setName(generateEventName());
            _view->update(_event, uuid, well->getName());
          });

  connect(_view.get(), &View::sendPicksInfo,
          [this](const auto type, const auto num, const auto l_val,
                 const auto pick_val, const auto r_val) {
            int idx = 0;
            for (auto &component : this->_event->getComponents()) {
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

  connect(_view.get(), &View::undoClicked, [this]() {
    std::cout << "event-address == " << _event.get() << std::endl;
    _undoStack->undo();
    //    _appliedOperations->undo();
    _view->update(_event);
  });
  connect(_view.get(), &View::redoClicked, [this]() {
    //    _appliedOperations->redo();
    std::cout << "event-address == " << _event.get() << std::endl;
    _undoStack->redo();
    _view->update(_event);
  });

  connect(
      _view.get(), &View::eventTransformClicked, [this, &wells_map](auto oper) {
        switch (oper) {
        case SeismEvent::RotateDataToEBasis:
          //              _appliedOperations->push(
          //                  new Modefication::RotateDataToEBasis(_event,
          //                  wells_map));
          _undoStack->push(
              new Modefication::RotateDataToEBasis(_event.get(), wells_map));
          break;
        case SeismEvent::TestMultiplier:
          std::cout << "event-address == " << _event.get() << std::endl;
          _undoStack->push(new Modefication::TestMultiplier(_event.get(), 5.0));
          break;
        }

        _view->update(_event);
      });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

Controller::Controller(
    const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &all_events,
    const std::map<QUuid, std::unique_ptr<Data::SeismWell>> &wells_map,
    std::unique_ptr<Data::SeismEvent> &event,
    std::unique_ptr<QUndoStack> &undoStack, QObject *parent)
    : QObject(parent), _model(nullptr), _event(std::move(event)),
      //      _event(std::make_unique<Data::SeismEvent>(*event)),
      _undoStack(std::move(undoStack))
//      _appliedOperations(new QUndoStack())
{

  //    _eventNameContainer[QUuid()] = _event->getName(); // TODO: it`s OK?

  // prepare data for view
  std::set<QString> eventNames;
  for (auto &uuid_event : all_events) {
    if (_event->getUuid() == uuid_event.first) {
      continue;
    }
    const auto &name = uuid_event.second->getName();
    if (name != _event->getName()) {
      eventNames.insert(uuid_event.second->getName());
    }
  }
  _view = std::make_unique<View>(eventNames, _event, _undoStack);
  // ...

  connect(_event.get(), &Data::SeismEvent::changed, []() {
    //              std::cout << "event changed" << std::endl;
  });

  connect(_view.get(), &View::createPolarizationAnalysisWindow, [this]() {
    _polarizationWindow = new PolarizationAnalysisWindow(_event);
    _polarizationWindow->show();
  });

  connect(_view.get(), &View::sendPicksInfo,
          [this](const auto type, const auto num, const auto l_val,
                 const auto pick_val, const auto r_val) {
            int idx = 0;
            for (auto &component : this->_event->getComponents()) {
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

  //  connect(_view.get(), &View::dataToEBasisClicked,
  //          [this]() { EventTools::dataToEBasis(_event); });

  connect(_view.get(), &View::undoClicked, [this]() {
    std::cout << "event-address == " << _event.get() << std::endl;
    _undoStack->undo();
    //    _appliedOperations->undo();
    _view->update(_event);
  });
  connect(_view.get(), &View::redoClicked, [this]() {
    //    _appliedOperations->redo();
    std::cout << "event-address == " << _event.get() << std::endl;
    _undoStack->redo();
    _view->update(_event);
  });

  connect(
      _view.get(), &View::eventTransformClicked, [this, &wells_map](auto oper) {
        switch (oper) {
        case SeismEvent::RotateDataToEBasis:
          //              _appliedOperations->push(
          //                  new Modefication::RotateDataToEBasis(_event,
          //                  wells_map));
          std::cout << "here" << std::endl;
          _undoStack->push(
              new Modefication::RotateDataToEBasis(_event.get(), wells_map));
          break;
        case SeismEvent::TestMultiplier:
          std::cout << "event-address == " << _event.get() << std::endl;
          _undoStack->push(new Modefication::TestMultiplier(_event.get(), 5.0));
          break;
        }

        _view->update(_event);
      });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::start() {
  //  _view->setModal(true); // TODO: uncomment
  _view->show();
}

void Controller::finish(int result) {
  auto &uuid = _event->getUuid();
  if (QDialog::Accepted == result) {
    _view->settingEventInfo(_event);
    // TODO: correctly!
    emit sendEventAndStack(_event, _undoStack);
  }
  emit sendStack(uuid, _undoStack);
  emit sendEvent(_event);

  emit finished();
}

QString Controller::generateEventName() const {
  QString name;
  int idx = 0;
  for (auto &uuid_word : _eventNameContainer) {
    if (0 != idx) {
      name += " --- ";
    }
    name += uuid_word.second;
    ++idx;
  }
  return name;
}

} // namespace OneEvent
} // namespace EventOperation
