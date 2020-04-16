#include "controller.h"

#include "data/seismevent.h"
#include "data/seismwell.h"
#include "event_operation/share/model.h"
#include "event_operation/share/view/event_view/analysis_view/analysiswindow.h"
//#include "view/view.h"

#include "event_operation/modification/commands/testindividualcommand.h"
#include "event_operation/modification/undocommandgetter.h"

#include "data/io/segyreader.h"

#include <QUndoStack>

#include <iostream> // TODO: remove

#include "data/projectsettings.h"

typedef Data::IO::SegyReader SegyReader;
typedef Data::SeismEvent SeismEvent;

namespace EventOperation {
namespace OneEvent {
Controller::Controller(
    const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &all_events,
    const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &wells_map,
    const std::list<std::shared_ptr<Data::SeismReceiver>> &receivers,
    QObject *parent)
    : QObject(parent), _model(new Model(new SegyReader(), this)),
      _event(std::make_shared<SeismEvent>())
//    ,_undoStack(std::make_shared<CustomIndividualUndoStack>())
{

  // prepare data for view
  std::map<QUuid, QString> wellNames_map;
  for (auto &uuid_well : wells_map) {
    wellNames_map[uuid_well.first] = uuid_well.second->getName();
  }
  std::set<QString> eventNames;
  for (auto &uuid_event : all_events) {
    eventNames.insert(uuid_event.second->getInfo().getName());
  }

  //  _view = std::make_unique<View>(eventNames, wellNames_map,
  //  _undoStack.get());

  connect(_view.get(), &View::infoChanged,
          [this] { _view->settingEventInfo(_event.get()); });

  connect(_event.get(), &Data::SeismEvent::infoChanged,
          [this](auto event) { _view->updateInfoEvent(event); });

  connect(_event.get(), &Data::SeismEvent::dataChanged,
          [this](auto event) { _view->updateDataEvent(event); });

  connect(_model, &Model::notify,
          [this](auto &msg) { _view->setNotification(msg); });

  connect(_view.get(), &View::sendWellUuidAndFilePath,
          [this, &wells_map, &receivers](auto &wellUuid, auto &filePath) {
            std::list<std::shared_ptr<Data::SeismReceiver>> receiversByWell;
            for (auto &receiver : receivers) {
              if (wellUuid == receiver->getSourseWell()->getUuid()) {
                receiversByWell.push_back(receiver);
              }
            }

            auto components =
                _model->getSeismComponents(receiversByWell, filePath);

            if (!components.empty()) {
              for (auto &component : components) {
                _event->addComponent(std::move(component));
              }
              _eventNameContainer[wellUuid] = QFileInfo(filePath).baseName();

              auto info = _event->getInfo();
              info.setName(generateEventName());
              _event->setInfo(info);

              _view->update(_event.get(), wellUuid);
            }
          });

  connect(_view.get(), &View::createAnalysisWindow, [this]() {
    _analysisWindow = new AnalysisWindow(
        _event); // TODO: может тоже отдавть (const * const) ?
    _analysisWindow->show();
  });

  connect(_view.get(), &View::sendWellUuidForRemove,
          [this, &wells_map, &receivers](auto &uuid) {
            for (auto &receiver : receivers) {
              if (uuid == receiver->getSourseWell()->getUuid()) {
                _event->removeComponentByReceiverUuid(receiver->getUuid());
              }
            }
            _eventNameContainer.erase(uuid);

            auto info = _event->getInfo();
            info.setName(generateEventName());
            _event->setInfo(info);

            auto &well = wells_map.at(uuid);
            _view->update(_event.get(), uuid, well->getName());
          });

  connect(_view.get(), &View::sendPicksInfo,
          [this](const auto type, const auto num, const auto l_val,
                 const auto pick_val, const auto r_val) {
            int idx = 0;
            for (auto &component : this->_event->getComponents()) {
              if (num == idx) {
                auto &picks_map = component->getWavePicks();
                auto itr_pic = picks_map.find(type);
                if (itr_pic != picks_map.end()) {
                  auto &pick = itr_pic->second;
                  pick.setValidDataStatus(false);
                  pick.setArrival(pick_val);
                  pick.setPolarizationLeftBorder(l_val);
                  pick.setPolarizationRightBorder(r_val);
                } else {
                  auto pick = Data::SeismWavePick(type, pick_val);
                  pick.setPolarizationLeftBorder(l_val);
                  pick.setPolarizationRightBorder(r_val);
                  component->addWavePick(pick);
                }
                _event->changeTrigger();
                break;
              }
              ++idx;
            }
          });

  connect(_view.get(), &View::eventTransformClicked,
          [this, &wells_map](auto oper) {
            //            CustomIndividualUndoCommand *command =
            //                UndoCommandGetter::get(oper, QUuid(),
            //                _event.get());
            //            _undoStack->push(command);
          });

  connect(_view.get(), &View::finished, this, &Controller::finish);
}

void Controller::start() {
  //  _view->setModal(true); // TODO: uncomment
  _view->show();
}

void Controller::finish(int result) {
  if (QDialog::Accepted == result) {
    //    emit sendEventAndStack(_event, _undoStack);
  }

  emit finished();
}

bool Controller::checkPolarizationAnalysisDataValid() {
  for (auto &component : _event->getComponents()) {
    for (auto &pick : component->getWavePicks()) {
      if (!pick.second.getValidDataStatus()) {
        return false;
      }
    }
  }
  return true;
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