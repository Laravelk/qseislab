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
// class SeismEvent;
class SeismWell;
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
                QWidget *parent = nullptr);

  void loadEvent(Data::SeismEvent const *const, QUndoStack const *const);
  void unloadEvent(QUndoStack const *const);

  void updateInfoEvent(Data::SeismEvent const *const);
  void updateDataEvent(Data::SeismEvent const *const);

  void update(const std::map<QUuid, std::shared_ptr<Data::SeismEvent>> &);

  void setNotification(const QString &);
  void settingEventInfo(Data::SeismEvent *const) const;
  ChartGesture *getChartGesture();

  void setAddPolarizationWindowButtonEnable(bool enable);
  PolarGraph *getPolarGraph() { return _polarGraph; }
  void showWarningWindowAboutValidStatusOfPolarizationAnalysisData();

signals:
  void infoChanged() const;

  void hideCurrentEvent();
  void changeCurrentEvent(const QUuid &);
  void removeEvent(const QUuid &);

  void sendWellUuidAndFilePaths(const QUuid &, const QStringList &) const;

  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);

  void undoClicked() const;
  void redoClicked() const;

  // tool-signals
  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;

  void removePick(Data::SeismWavePick::Type, int);
  void createPolarizationAnalysisWindow();
  void calculatePolarizationAnalysisData();
  void clickOnPolarAnalysisInGraph();

private slots:
  void recvFilesPath(const QStringList &);

private:
  void addLocal(const QString &);
  void removeLocal(const QString &);
  QBrush updateRepetition(const QString &);
  bool allValid() const;

  QPushButton *_undoButton;
  QPushButton *_redoButton;

  EventToolsWidget *_toolsWidget;

  InfoEvent *_infoEvent;
  QComboBox *_wellNames;
  QUuid _wellUuid;
  QFileDialog *_fileDialog;

  QTabWidget *_tabWidget;

  QListWidget *_eventList;

  GraphicController *_graphicEvent;

  PolarGraph *_polarGraph;

  QPushButton *_okButton;
  QPushButton *_cancelButton;

  std::set<QString> _globalEventNames;
  std::map<QString, int> _localEventNames;

  const int GRAPH_INDEX_IN_TAB = 0;
  const int POLAR_ANALYSIS_INDEX_IN_TAB = 1;
};

} // namespace MoreEvents
} // namespace EventOperation
