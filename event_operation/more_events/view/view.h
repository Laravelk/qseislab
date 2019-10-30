#pragma once

#include "data/seismwavepick.h"

#include <QBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QFileDialog>
#include <QListWidget>
#include <QUuid>

#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class InfoEvent;
class GraphicController;
class ChartGesture;
namespace MoreEvents {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(const std::map<QUuid, QString> &, QWidget *parent = nullptr);

  //  void update(const std::unique_ptr<Data::SeismEvent> &, const QUuid &);
  //  void update(const std::unique_ptr<Data::SeismEvent> &, const QUuid &,
  //              const QString &);
  void loadEvent(const std::unique_ptr<Data::SeismEvent> &);
  void unloadEvent();

  void update(const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &);

  void setNotification(const QString &);
  void settingEventInfo(const std::unique_ptr<Data::SeismEvent> &) const;
  ChartGesture *getChartGesture();

signals:
  void hideCurrentEvent();
  void changeCurrentEvent(const QUuid &);
  void removeEvent(const QUuid &);

  void sendWellUuidAndFilePaths(const QUuid &, const QStringList &) const;
  //  void sendWellUuidForRemove(const QUuid &) const;
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void createPolarizationAnalysisWindow();

private slots:
  void recvFilesPath(const QStringList &);

private:
  InfoEvent *_infoEvent;
  QComboBox *_wellNames;
  QUuid _wellUuid;
  QFileDialog *_fileDialog;

  QListWidget *_eventList;

  //  QVBoxLayout *_wellManagersLayout;
  //  QPushButton *_addButtonManagers;
  GraphicController *_graphicEvent;
  QPushButton *_okButton;
  QPushButton *_cancelButton;
  QPushButton *_addWaveButton;
  QPushButton *_polarizationEventButton;

  QAction *_addPWave;
  QAction *_addSWave;

  //  std::map<QUuid, QString> _wellNames_map;
};

} // namespace MoreEvents
} // namespace EventOperation
