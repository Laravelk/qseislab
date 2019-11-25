#include "controller.h"

#include "data/seismevent.h"
#include "data/seismwell.h"
#include "event_operation/share/model.h"
#include "event_operation/share/view/3dscene/polarizationanalysiswindow.h"
//#include "view/view.h"

#include "event_operation/modification/rotatedatatoebasis.h"
#include "event_operation/modification/testmultiplier.h"
#include "undo_stack_work/event_modification/undocommandgetter.h"

#include "data/io/segyreader.h"

#include <QUndoStack>

#include <iostream> // TODO: remove

typedef Data::IO::SegyReader SegyReader;
typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace OneEvent {
Controller::Controller(
    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &all_events,
    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map,
    QObject *parent)
    : QObject(parent), _model(new Model(new SegyReader(), this)),
      _event(std::make_shared<SeismEvent>()),
      _undoStack(std::make_shared<CustomIndividualUndoStack>()) {

  // prepare data for view
  std::map<QUuid, QString> wellNames_map;
  for (auto &uuid_well : wells_map) {
    wellNames_map[uuid_well.first] = uuid_well.second->getName();
  }
  std::set<QString> eventNames;
  for (auto &uuid_event : all_events) {
    eventNames.insert(uuid_event.second->getInfo()->getName());
  }
  // TODO: uncomment
  _view = std::make_unique<View>(eventNames, wellNames_map, _undoStack.get());
  // ...

  connect(_event.get(), &Data::SeismEvent::infoChanged, [this]() {
    _view->update(_event.get());
    std::cout << "event info changed" << std::endl;
  });

  connect(_event.get(), &Data::SeismEvent::dataChanged, [this]() {
    _view->update(_event.get());
    std::cout << "event info changed" << std::endl;
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
              // TODO: implement!
              //              _event->setName(generateEventName());

              _view->update(_event.get(), wellUuid);
            }
          });
  connect(_view.get(), &View::createPolarizationAnalysisWindow, [this]() {
    _polarizationWindow = new PolarizationAnalysisWindow(
        _event); // TODO: может тоже отдавть (const * const) ?
    _polarizationWindow->show();
  });

  connect(_view.get(), &View::sendWellUuidForRemove,
          [this, &wells_map](auto &uuid) {
            auto &well = wells_map.at(uuid);
            for (auto &reciever : well->getReceivers()) {
              _event->removeComponentByReceiverUuid(reciever->getUuid());
            }
            _eventNameContainer.erase(uuid);
            // TODO: implement!
            //            _event->setName(generateEventName());

            _view->update(_event.get(), uuid, well->getName());
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
    _undoStack->undo();
    _view->update(_event.get());
  });
  connect(_view.get(), &View::redoClicked, [this]() {
    _undoStack->redo();
    _view->update(_event.get());
  });

  connect(_view.get(), &View::eventTransformClicked,
          [this, &wells_map](auto oper) {
            //        switch (oper) {
            //        case SeismEvent::RotateDataToEBasis:
            //          //              _appliedOperations->push(
            //          //                  new
            //          Modefication::RotateDataToEBasis(_event,
            //          //                  wells_map));
            //          _undoStack->push(
            //              new Modefication::RotateDataToEBasis(_event.get(),
            //              wells_map));
            //          break;
            //        case SeismEvent::TestMultiplier:
            //          std::cout << "event-address == " << _event.get() <<
            //          std::endl; _undoStack->push(new
            //          Modefication::TestMultiplier(_event.get(), 5.0)); break;
            //        }

            CustomIndividualUndoCommand *command =
                UndoCommandGetter::get(oper, QUuid(), _event.get());
            _undoStack->push(command);

            _view->update(_event.get());
          });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

// Controller::Controller(
//    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &all_events,
//    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map,
//    const std::shared_ptr<Data::SeismEvent> &event,
//    const std::shared_ptr<CustomIndividualUndoStack> &undoStack,
//    QObject *parent)
//    : QObject(parent), _model(nullptr), _event(event), _undoStack(undoStack) {

//  //    _eventNameContainer[QUuid()] = _event->getName(); // TODO: it`s OK?

//  // prepare data for view
//  std::set<QString> eventNames;
//  for (auto &uuid_event : all_events) {
//    if (_event->getUuid() == uuid_event.first) {
//      continue;
//    }
//    const auto &name = uuid_event.second->getName();
//    if (name != _event->getName()) {
//      eventNames.insert(uuid_event.second->getName());
//    }
//  }
//  // TODO: uncomment
//  _view = std::make_unique<View>(eventNames, _event.get(), _undoStack.get());
//  // ...

//  connect(_event.get(), &Data::SeismEvent::changed, []() {
//    //              std::cout << "event changed" << std::endl;
//  });

//  connect(_view.get(), &View::createPolarizationAnalysisWindow, [this]() {
//    _polarizationWindow = new PolarizationAnalysisWindow(_event);
//    _polarizationWindow->show();
//  });

//  connect(_view.get(), &View::sendPicksInfo,
//          [this](const auto type, const auto num, const auto l_val,
//                 const auto pick_val, const auto r_val) {
//            int idx = 0;
//            for (auto &component : this->_event->getComponents()) {
//              if (num == idx) {
//                Data::SeismWavePick wavePick =
//                    Data::SeismWavePick(type, pick_val);
//                wavePick.setPolarizationLeftBorder(l_val);
//                wavePick.setPolarizationRightBorder(r_val);
//                component->addWavePick(wavePick);
//                break;
//              }
//              ++idx;
//            }
//          });

//  //  connect(_view.get(), &View::dataToEBasisClicked,
//  //          [this]() { EventTools::dataToEBasis(_event); });

//  connect(_view.get(), &View::undoClicked, [this]() {
//    _undoStack->undo();
//    _view->update(_event.get());
//  });
//  connect(_view.get(), &View::redoClicked, [this]() {
//    _undoStack->redo();
//    _view->update(_event.get());
//  });

//  connect(_view.get(), &View::eventTransformClicked,
//          [this, &wells_map](auto oper) {
//            //        switch (oper) {
//            //        case SeismEvent::RotateDataToEBasis:
//            //          //              _appliedOperations->push(
//            //          //                  new
//            //          Modefication::RotateDataToEBasis(_event,
//            //          //                  wells_map));
//            //          std::cout << "here" << std::endl;
//            //          _undoStack->push(
//            //              new Modefication::RotateDataToEBasis(_event.get(),
//            //              wells_map));
//            //          break;
//            //        case SeismEvent::TestMultiplier:
//            //          std::cout << "event-address == " << _event.get() <<
//            //          std::endl; _undoStack->push(new
//            //          Modefication::TestMultiplier(_event.get(), 5.0));
//            break;
//            //        }

//            CustomIndividualUndoCommand *command =
//                UndoCommandGetter::get(oper, QUuid(), _event.get());
//            _undoStack->push(command);

//            _view->update(_event.get());
//          });

//  connect(_view.get(), &View::finished, this, &Controller::finish);
//}

// QWidget *Controller::getView() { return _view.get(); }

void Controller::start() {
  //  _view->setModal(true); // TODO: uncomment
  _view->show();
}

void Controller::finish(int result) {
  //  auto &uuid = _event->getUuid();
  if (QDialog::Accepted == result) {
    _view->settingEventInfo(_event.get());
    emit sendEventAndStack(_event, _undoStack);
  }

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
