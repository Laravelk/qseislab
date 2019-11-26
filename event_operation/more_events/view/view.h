#pragma once

<<<<<<< HEAD
#include "data/seismevent.h"
=======
>>>>>>> test
#include "data/seismwavepick.h"

#include <QBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QFileDialog>
#include <QListWidget>
<<<<<<< HEAD
#include <QUndoStack>
=======
>>>>>>> test
#include <QUuid>

#include <memory>
#include <set>

namespace Data {
<<<<<<< HEAD
// class SeismEvent;
=======
class SeismEvent;
>>>>>>> test
class SeismWell;
} // namespace Data

namespace EventOperation {
<<<<<<< HEAD
class EventToolsWidget;
class InfoEvent;
class GraphicController;
=======
class InfoEvent;
class GraphicController;
class PolarGraph;
>>>>>>> test
class ChartGesture;
namespace MoreEvents {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(const std::set<QString> &, const std::map<QUuid, QString> &,
                QWidget *parent = nullptr);

<<<<<<< HEAD
  void loadEvent(Data::SeismEvent const *const, QUndoStack const *const);
  void unloadEvent(QUndoStack const *const);

  void updateInfoEvent(Data::SeismEvent const *const);
  void updateDataEvent(Data::SeismEvent const *const);

  void update(const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &);

  void setNotification(const QString &);
  void settingEventInfo(Data::SeismEvent *const) const;
  ChartGesture *getChartGesture();

signals:
  void infoChanged() const;

=======
  void loadEvent(const std::unique_ptr<Data::SeismEvent> &);
  void unloadEvent();
  void setAddPolarizationWindowButtonEnable(bool enable);

  void update(const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &);
  PolarGraph *getPolarGraph() { return _polarGraph; }

  void setNotification(const QString &);
  void settingEventInfo(const std::unique_ptr<Data::SeismEvent> &) const;
  ChartGesture *getChartGesture();

signals:
>>>>>>> test
  void hideCurrentEvent();
  void changeCurrentEvent(const QUuid &);
  void removeEvent(const QUuid &);

  void sendWellUuidAndFilePaths(const QUuid &, const QStringList &) const;

  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
<<<<<<< HEAD
  void createPolarizationAnalysisWindow();

  void undoClicked() const;
  void redoClicked() const;

  // tool-signals
  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;
=======
  void removePick(Data::SeismWavePick::Type, int);
  void createPolarizationAnalysisWindow();
  void calculatePolarizationAnalysisData();
>>>>>>> test

private slots:
  void recvFilesPath(const QStringList &);

private:
  void addLocal(const QString &);
  void removeLocal(const QString &);
  QBrush updateRepetition(const QString &);
  bool allValid() const;

<<<<<<< HEAD
  QPushButton *_undoButton;
  QPushButton *_redoButton;

  EventToolsWidget *_toolsWidget;

=======
>>>>>>> test
  InfoEvent *_infoEvent;
  QComboBox *_wellNames;
  QUuid _wellUuid;
  QFileDialog *_fileDialog;
<<<<<<< HEAD
=======
  QTabWidget *_tabWidget;
>>>>>>> test

  QListWidget *_eventList;

  GraphicController *_graphicEvent;
<<<<<<< HEAD
=======
  PolarGraph *_polarGraph;
>>>>>>> test
  QPushButton *_okButton;
  QPushButton *_cancelButton;

  std::set<QString> _globalEventNames;
  std::map<QString, int> _localEventNames;
};

} // namespace MoreEvents
} // namespace EventOperation
