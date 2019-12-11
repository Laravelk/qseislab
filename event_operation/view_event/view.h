#pragma once

#include "data/seismevent.h"
#include "data/seismwavepick.h"

#include <QWidget>

#include <QUndoStack>
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
class ChartGesture;
namespace ViewEvent {
class View : public QWidget {
  Q_OBJECT

public:
  explicit View(const std::set<QString> &, Data::SeismEvent const *const,
                QUndoStack const *const, QWidget *parent = nullptr);

  //  void update(Data::SeismEvent const *const);
  bool allValid() const;

  void updateInfoEvent(Data::SeismEvent const *const);
  void updateDataEvent(Data::SeismEvent const *const);

  void settingEventInfo(Data::SeismEvent *const) const;
  ChartGesture *getChartGesture();

signals:
  void infoChanged() const;

  void eventActionClicked(Data::SeismEvent::TransformOperation) const;
  void undoClicked() const;
  void redoClicked() const;

  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void createPolarizationAnalysisWindow();

  void finished() const;

  //  void captureFocus() const;

  // protected:
  //  void focusInEvent(QFocusEvent *event) override;

private:
  void updateRepetition(const QString &);

  EventToolsWidget *_toolsWidget;

  InfoEvent *_infoEvent;

  GraphicController *_graphicEvent;

  std::map<QUuid, QString> _wellNames_map;
  std::set<QString> _eventNames;
  bool _isValid;
};

} // namespace ViewEvent
} // namespace EventOperation
