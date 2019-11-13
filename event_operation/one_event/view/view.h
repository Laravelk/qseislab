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
#include <QTableWidget>
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
  explicit View(const std::set<QString> &,
                const std::shared_ptr<Data::SeismEvent> &,
                const std::shared_ptr<QUndoStack> &, QWidget *parent = nullptr);
  explicit View(const std::set<QString> &, const std::map<QUuid, QString> &,
                const std::shared_ptr<QUndoStack> &, QWidget *parent = nullptr);

  void update(const std::shared_ptr<Data::SeismEvent> &);

  void update(const std::shared_ptr<Data::SeismEvent> &, const QUuid &);
  void update(const std::shared_ptr<Data::SeismEvent> &, const QUuid &,
              const QString &);
  void setNotification(const QString &);
  void settingEventInfo(const std::shared_ptr<Data::SeismEvent> &) const;
  ChartGesture *getChartGesture();

signals:
  void sendWellUuidAndFilePath(const QUuid &, const QString &) const;
  void sendWellUuidForRemove(const QUuid &) const;
  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void createPolarizationAnalysisWindow();

  void undoClicked() const;
  void redoClicked() const;

  // tool-signals
  void eventTransformClicked(Data::SeismEvent::TransformOperation) const;

private:
  void commonSetting();
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
