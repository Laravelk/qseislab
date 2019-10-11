#pragma once

#include "data/seismwavepick.h"

#include <QDialog>

#include <QBoxLayout>
#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class InfoEvent;
class Controller;
namespace AddEvent {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(const std::map<QUuid, QString> &, QWidget *parent = nullptr);

  void update(const std::unique_ptr<Data::SeismEvent> &, const QUuid &);
  void update(const std::unique_ptr<Data::SeismEvent> &, const QUuid &,
              const QString &);
  void setNotification(const QString &);

  void settingEventInfo(const std::unique_ptr<Data::SeismEvent> &) const;

signals:
  void sendWellUuidAndFilePath(const QPair<QUuid, QString> &) const;
  void sendWellUuidForRemove(const QUuid &) const;

private:
  InfoEvent *_infoEvent;
  QVBoxLayout *_wellManegersLayout;
  QPushButton *_addButtonManagers;
  Controller *_graphicEvent;
  QPushButton *_okButton;
  QPushButton *_cancelButton;
  QPushButton *_addWaveButton;

  QAction *_addPWave;
  QAction *_addSWave;

  std::map<QUuid, QString> _wellNames_map;
};

} // namespace AddEvent
} // namespace EventOperation
