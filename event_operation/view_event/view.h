#pragma once

#include "data/seismwavepick.h"

#include <QDialog>

#include <memory>

namespace Data {
class SeismEvent;
}

namespace EventOperation {
class InfoEvent;
class Controller;
namespace ViewEvent {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void update(const std::unique_ptr<Data::SeismEvent> &);

  void settingEventInfo(const std::unique_ptr<Data::SeismEvent> &) const;

signals:
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);

private:
  InfoEvent *_infoEvent;
  Controller *_graphicEvent; // share_view controller
  QPushButton *_okButton;
  QPushButton *_cancelButton;
  QPushButton *_addWaveButton;

  QAction *_addPWave;
  QAction *_addSWave;
};

} // namespace ViewEvent
} // namespace EventOperation
