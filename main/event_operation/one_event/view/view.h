#pragma once

#include "data/seismevent.h"
#include "data/seismwavepick.h"

#include <QBoxLayout>
#include <QDialog>

#include <memory>
#include <set>

#include <QLabel>
#include <QRadioButton>
#include <QSlider>
#include <QUndoStack>

namespace Data {
// class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class EventToolsWidget;
class InfoEvent;
class GraphicController;
class ChartGesture;
namespace OneEvent {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(const std::set<QString> &, const std::map<QUuid, QString> &,
                QUndoStack const *const, QWidget *parent = nullptr);

  void updateInfoEvent(Data::SeismEvent const *const);
  void updateDataEvent(Data::SeismEvent const *const);

  void update(Data::SeismEvent const *const, const QUuid &);
  void update(Data::SeismEvent const *const, const QUuid &, const QString &);

  void setNotification(const QString &);
  void settingEventInfo(Data::SeismEvent *const) const;
  ChartGesture *getChartGesture();

signals:
  void infoChanged() const;

  void sendWellUuidAndFilePath(const QUuid &, const QString &) const;
  void sendWellUuidForRemove(const QUuid &) const;

  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void addPick(Data::SeismWavePick::Type, int, int, int, int);
  void removePick(Data::SeismWavePick::Type, int);

  void calculatePolarizationAnalysisData();
  void createAnalysisWindow();

  void undoClicked() const;
  void redoClicked() const;

  // tool-signals
  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;

private:
  void updateRepetition(const QString &);

  EventToolsWidget *_toolsWidget;

  InfoEvent *_infoEvent;
  QVBoxLayout *_wellManagersLayout;
  QPushButton *_addButtonManagers;

  GraphicController *_graphicEvent;

  QPushButton *_okButton;
  QPushButton *_cancelButton;

  std::map<QUuid, QString> _wellNames_map;
  std::set<QString> _eventNames;
  bool _isValid;
};

} // namespace OneEvent
} // namespace EventOperation
