#pragma once

#include "data/seismevent.h"
#include "data/seismwavepick.h"

#include <QBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QFileDialog>
#include <QListWidget>
#include <QUndoStack>
#include <QUuid>

#include <memory>
#include <set>

namespace Data {
class SeismEvent;
class SeismWell;
class SeismReceiver;
class ProjectSettings;
} // namespace Data

namespace EventOperation {
class EventToolsWidget;
class InfoEvent;
class GraphicController;
class PolarGraph;
class ChartGesture;
namespace MoreEvents {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(const std::set<QString> &, const std::map<QUuid, QString> &,
                QUndoStack const *const, QWidget *parent = nullptr);

  void loadEvent(Data::SeismEvent const *const);
  void unloadEvent();

  void updateInfoEvent(Data::SeismEvent const *const);
  void updateDataEvent(Data::SeismEvent const *const);

  void update(const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &);

  void setNotification(const QString &);
  void settingEventInfo(Data::SeismEvent *const) const;
  ChartGesture *getChartGesture();

  void setAddPolarizationWindowButtonEnable(bool enable);

signals:
  void infoChanged() const;

  void hideCurrentEvent();
  void changeCurrentEvent(const QUuid &);
  void removeEvent(const QUuid &);

  void sendWellUuidAndFilePaths(const QUuid &, const QStringList &) const;

  // tool-signals
  void undoClicked() const;
  void redoClicked() const;
  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;

  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void removePick(Data::SeismWavePick::Type, int);
  void addPick(Data::SeismWavePick::Type, int, int, int, int);

  void calculatePolarizationAnalysisData();
  void createAnalysisWindow();
  void updatePolarGraphSignal();

private slots:
  void recvFilesPath(const QStringList &);

private:
  void addLocal(const QString &);
  void removeLocal(const QString &);
  QBrush updateRepetition(const QString &);
  bool allValid() const;

  EventToolsWidget *_toolsWidget;

  InfoEvent *_infoEvent;
  QComboBox *_wellNames;
  QUuid _wellUuid;
  QFileDialog *_fileDialog;

  QListWidget *_eventList;

  GraphicController *_graphicEvent;

  QPushButton *_okButton;
  QPushButton *_cancelButton;

  std::set<QString> _globalEventNames;
  std::map<QString, int> _localEventNames;
};

} // namespace MoreEvents
} // namespace EventOperation