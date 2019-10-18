#include "view.h"

#include "event_operation/share_view/controller.h"
#include "event_operation/share_view/infoevent.h"

#include <QBoxLayout>
#include <QPushButton>

#include <iostream> // TODO: remove

namespace EventOperation {
namespace ViewEvent {
View::View(QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint),
      _infoEvent(new InfoEvent(this)), _graphicEvent(new Controller(this)),
      _okButton(new QPushButton("Ok", this)),
      _cancelButton(new QPushButton("Cancel", this)),
      _addWaveButton(new QPushButton("+", this)) {

  // Setting`s
  setWindowTitle("SeismWindow");
  setMinimumSize(1100, 590);

  QMenu *addWaveButtonMenu = new QMenu(_addWaveButton);
  _addPWave = new QAction("PWAVE", _addWaveButton);
  _addSWave = new QAction("SWAVE", _addWaveButton);
  addWaveButtonMenu->addAction(_addPWave);
  addWaveButtonMenu->addAction(_addSWave);
  _addWaveButton->setMenu(addWaveButtonMenu);
  // Setting`s end

  // Connecting
  connect(_graphicEvent, &EventOperation::Controller::sendPicksInfo,
          [this](auto type, auto num, auto l_val, auto pick_val, auto r_val) {
            emit sendPicksInfo(type, num, l_val, pick_val, r_val);
          });
  connect(_addPWave, &QAction::triggered, [this]() {
    _graphicEvent->getView()->setWaveAddTriggerFlag(Data::SeismWavePick::PWAVE);
  });
  connect(_addSWave, &QAction::triggered, [this]() {
    _graphicEvent->getView()->setWaveAddTriggerFlag(Data::SeismWavePick::SWAVE);
  });
  connect(_okButton, &QPushButton::clicked, this, &View::accept);
  connect(_cancelButton, &QPushButton::clicked, this, &View::reject);
  // Connecting end

  // Layout`s
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(_infoEvent);
  leftLayout->addStretch(1);

  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(_okButton);
  buttonsLayout->addWidget(_cancelButton);
  buttonsLayout->addWidget(_addWaveButton);

  QVBoxLayout *graphicLayout = new QVBoxLayout();
  graphicLayout->addWidget(_graphicEvent->getView(), 10);
  graphicLayout->addStretch(1);
  graphicLayout->addLayout(buttonsLayout);

  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addLayout(leftLayout);
  //    mainLayout->addStretch(1);
  mainLayout->addLayout(graphicLayout, 10);

  setLayout(mainLayout);
  // Layout`s end
}

void View::update(const std::unique_ptr<Data::SeismEvent> &event) {
  _infoEvent->update(event);
  _graphicEvent->update(event);
}

void View::settingEventInfo(
    const std::unique_ptr<Data::SeismEvent> &event) const {
  _infoEvent->settingEventInfo(event);
}

} // namespace ViewEvent
} // namespace EventOperation
